/* S1 Q6: LED1 = PB0, LED2 = PB1, button = PA0.
 *   1 press: LED1 on, LED2 off | 2 presses: both on | 3 presses: LED1 off, LED2 on
 * Same press counting as Q3e. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PRESSED (gpio_get(GPIOA, GPIO0) == 0)
void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    int count = 0, idle = 0;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0 | GPIO1);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
    gpio_set(GPIOA, GPIO0);                       /* pull-up */

    while (1) {
        if (PRESSED) {
            delay(20);
            while (PRESSED);
            delay(20);
            count++;
            idle = 0;
        }
        delay(1);
        idle++;

        if (count > 0 && idle > 700) {
            gpio_clear(GPIOB, GPIO0 | GPIO1);           /* start from both off */
            if (count == 1) gpio_set(GPIOB, GPIO0);
            if (count == 2) gpio_set(GPIOB, GPIO0 | GPIO1);
            if (count == 3) gpio_set(GPIOB, GPIO1);
            count = 0;
        }
    }
}
