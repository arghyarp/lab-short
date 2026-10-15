/* ADC: read internal temperature sensor, Vref, PA0, PA1 -> print in minicom.
 * PA0/PA1: connect a potentiometer middle pin (ends to 3.3V and GND).
 * raw 0..4095 -> mV: raw * 3300 / 4095
 * Temp (datasheet typical): T = (1430 mV - Vsense) / 4.3 mV + 25 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/* Wiring: PA9 (TX) -> RX of USB-TTL, GND -> GND.  minicom -D /dev/ttyUSB0 -b 115200 */

void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

void uart_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
    usart_set_baudrate(USART1, 115200);
    usart_set_mode(USART1, USART_MODE_TX);        /* 8N1 is the reset default */
    usart_enable(USART1);
}

void print(const char *s) { while (*s) usart_send_blocking(USART1, *s++); }

void print_num(int n)                             /* 123 -> "1","2","3" */
{
    if (n >= 10) print_num(n / 10);
    usart_send_blocking(USART1, '0' + n % 10);
}

#include <libopencm3/stm32/adc.h>

int read_mv(uint8_t channel)
{
    adc_set_regular_sequence(ADC1, 1, &channel);
    adc_start_conversion_direct(ADC1);
    while (!adc_eoc(ADC1));
    return adc_read_regular(ADC1) * 3300 / 4095;
}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();

    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0 | GPIO1);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);           /* 72/6 = 12 MHz (max 14) */
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_239DOT5CYC);
    adc_enable_temperature_sensor();
    adc_power_on(ADC1);
    delay(1);                                             /* let ADC settle */
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while (adc_is_calibrating(ADC1));

    while (1) {
        print("Temp: ");  print_num((1430 - read_mv(ADC_CHANNEL_TEMP)) * 10 / 43 + 25);
        print(" C  Vref: "); print_num(read_mv(ADC_CHANNEL_VREF));
        print(" mV  PA0: "); print_num(read_mv(0));
        print(" mV  PA1: "); print_num(read_mv(1));
        print(" mV\r\n");
        delay(1500);
    }
}
