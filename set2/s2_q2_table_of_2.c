/* S2 Q2: lookup table of 2. For 1..20, if the number is in the table,
 * print how many times 2 was multiplied (14 -> 2 x 7). */
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

int main(void)
{
    int table[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();

    while (1) {
        for (int n = 1; n <= 20; n++) {
            print_num(n);
            for (int k = 0; k < 10; k++) {
                if (table[k] == n) {
                    print(" = 2 x ");
                    print_num(k + 1);
                }
            }
            print("\r\n");
            delay(500);
        }
        delay(2000);
    }
}
