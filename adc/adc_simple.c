/* Internal temperature sensor, USART1 38400 8N1.
 * PA9 -> USB-UART RX, common GND. Assumes VDDA = 3.3 V. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/adc.h>

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
    usart_set_databits(USART1, 8);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_enable(USART1);
}

static void adc_setup(void)
{
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);     /* 72/6 = 12 MHz (max 14) */
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_239DOT5CYC);
    adc_enable_temperature_sensor();
    adc_power_on(ADC1);
    delay(1);                                       /* let ADC settle */
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while (adc_is_calibrating(ADC1));
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

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();
    adc_setup();

    uint8_t channel = ADC_CHANNEL_TEMP;
    adc_set_regular_sequence(ADC1, 1, &channel);

    while (1) {
        adc_start_conversion_direct(ADC1);
        while (!adc_eoc(ADC1));

        int raw = adc_read_regular(ADC1);
        int mv = raw * 3300 / 4095;
        int temp = (1430 - mv) * 10 / 43 + 25;

        print("Temperature: ");
        print_num(temp);
        print(" C\r\n");
        delay(1500);
    }
}
