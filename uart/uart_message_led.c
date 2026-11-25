/* STM32F103 Blue Pill, USART1 PA9 TX, 38400 8N1. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

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

static void print(const char *s)
{
    while (*s) usart_send_blocking(USART1, *s++);
}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13); /* LED off: active low */

    while (1) {
        print("Hello STM32!\r\n");
        gpio_toggle(GPIOC, GPIO13);
        delay(1000);
    }
}
