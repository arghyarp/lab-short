/* ADC: read internal temperature sensor, Vrefint, PA0, PA1 -> print in minicom.
 * PA0/PA1: potentiometer middle pin (ends to 3.3V and GND).
 * VDDA is measured at runtime from Vrefint (1.20 V typical), so the
 * conversion does not assume the supply is exactly 3.3 V.
 * Temp (datasheet typical): T = (1430 mV - Vsense) / 4.3 mV + 25 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/adc.h>

/* Wiring: PA9 (TX) -> RX of USB-TTL, GND -> GND.  minicom -D /dev/ttyUSB0 -b 38400 */

#define VREFINT_MV 1200                     /* typical Vrefint, datasheet */

static void delay(int ms)
{
    for (volatile int i = 0; i < ms * 6000; i++);
}

static void uart_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
    usart_set_baudrate(USART1, 38400);
    usart_set_mode(USART1, USART_MODE_TX);  /* 8N1 is the reset default */
    usart_enable(USART1);
}

static void print(const char *s)
{
    while (*s) usart_send_blocking(USART1, *s++);
}

static void print_num(int n)                /* 123 -> "1","2","3", -5 -> "-5" */
{
    if (n < 0) {
        usart_send_blocking(USART1, '-');
        n = -n;
    }
    if (n >= 10) print_num(n / 10);
    usart_send_blocking(USART1, '0' + n % 10);
}

static uint16_t read_raw(uint8_t channel)
{
    adc_set_regular_sequence(ADC1, 1, &channel);
    adc_start_conversion_direct(ADC1);
    while (!adc_eoc(ADC1));
    return adc_read_regular(ADC1);
}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();

    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0 | GPIO1);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);     /* 72/6 = 12 MHz (max 14) */
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_239DOT5CYC);
    adc_enable_temperature_sensor();
    adc_power_on(ADC1);
    delay(1);                                       /* let ADC settle */
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while (adc_is_calibrating(ADC1));

    while (1) {
        /* 1. Measure the real supply from the internal reference */
        uint16_t vref_raw = read_raw(ADC_CHANNEL_VREF);
        int vdda_mv = VREFINT_MV * 4095 / vref_raw;

        /* 2. Convert every other channel using the measured VDDA */
        int vsense_mv = read_raw(ADC_CHANNEL_TEMP) * vdda_mv / 4095;
        int temp_c    = (1430 - vsense_mv) * 10 / 43 + 25;   /* 4.3 mV/°C */
        int pa0_mv    = read_raw(0) * vdda_mv / 4095;
        int pa1_mv    = read_raw(1) * vdda_mv / 4095;

        print("Temp: ");  print_num(temp_c);
        print(" C  VDDA: "); print_num(vdda_mv);
        print(" mV  PA0: "); print_num(pa0_mv);
        print(" mV  PA1: "); print_num(pa1_mv);
        print(" mV\r\n");

        delay(1500);
    }
}
