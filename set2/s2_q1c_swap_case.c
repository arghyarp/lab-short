/* S2 Q1c: print the text with upper/lower case swapped; blink once per print.
 * ASCII: 'a' - 'A' = 32, so +32 makes lowercase, -32 makes uppercase. */
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

int main(void)
{
    const char *text = "Hello World 123";

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();
    rcc_periph_clock_enable(RCC_GPIOC);           /* on-board LED, active-low */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);                      /* off */

    while (1) {
        for (const char *p = text; *p; p++) {
            char c = *p;
            if (c >= 'A' && c <= 'Z')      c += 32;
            else if (c >= 'a' && c <= 'z') c -= 32;
            usart_send_blocking(USART1, c);
        }
        print("\r\n");
        blink(1);
        delay(1000);
    }
}
