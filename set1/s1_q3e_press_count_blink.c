/* S1 Q3e: press N times (quickly) -> LED blinks N times.
 * A burst ends when there is no press for 700 ms. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PRESSED (gpio_get(GPIOA, GPIO0) == 0)
void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    int count = 0, idle = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
    gpio_set(GPIOA, GPIO0);                       /* pull-up */

    while (1) {
        if (PRESSED) {
            delay(20);
            while (PRESSED);           /* wait for release */
            delay(20);
            count++;
            idle = 0;
        }
        delay(1);
        idle++;

        if (count > 0 && idle > 700) { /* burst finished */
            for (int i = 0; i < count; i++) {
                gpio_set(GPIOB, GPIO0);   delay(300);
                gpio_clear(GPIOB, GPIO0); delay(300);
            }
            count = 0;
        }
    }
}
