/* S2 Q1b: if the text contains 'A' blink twice, else once. */
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

void blink(int times)
{
    for (int i = 0; i < times; i++) {
        gpio_clear(GPIOC, GPIO13); delay(250);    /* on  */
        gpio_set(GPIOC, GPIO13);   delay(250);    /* off */
    }
}

int has_A(const char *s)
{
    while (*s) if (*s++ == 'A') return 1;
    return 0;
}

int main(void)
{
    const char *texts[] = {"hello world", "Apple", "BANANA", "banana"};

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();
    rcc_periph_clock_enable(RCC_GPIOC);           /* on-board LED, active-low */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);                      /* off */

    while (1) {
        for (int i = 0; i < 4; i++) {
            print(texts[i]);
            print("\r\n");
            blink(has_A(texts[i]) ? 2 : 1);
            delay(1000);
        }
    }
}
