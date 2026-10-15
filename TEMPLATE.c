/* MEMORISE THIS. Every GPIO question = this template + new logic in while(1).
 *
 * Pin "PXn"  ->  clock: RCC_GPIOX    port: GPIOX    pin: GPIOn
 *   e.g. PA5 ->  RCC_GPIOA, GPIOA, GPIO5      PB12 -> RCC_GPIOB, GPIOB, GPIO12
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PRESSED (gpio_get(GPIOA, GPIO0) == 0)     /* button to GND -> pressed reads 0 */

void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* LED (output): 1. clock on  2. set mode */
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

    /* Button (input): 1. clock on  2. set mode  3. choose pull-up */
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
    gpio_set(GPIOA, GPIO0);

    while (1) {
        /* logic goes here. The only 4 GPIO actions you ever need:
         *   gpio_set(GPIOB, GPIO0);      pin HIGH (LED on)
         *   gpio_clear(GPIOB, GPIO0);    pin LOW  (LED off)
         *   gpio_toggle(GPIOB, GPIO0);   flip it
         *   if (PRESSED) { ... }         read the button
         */
    }
}
