/* S1 Q3d: LED (PB0) turns on AFTER the button is released, stays on until next press. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PRESSED (gpio_get(GPIOA, GPIO0) == 0)
void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
    gpio_set(GPIOA, GPIO0);                       /* pull-up */
    gpio_clear(GPIOB, GPIO0);

    while (1) {
        while (!PRESSED)              /* wait for press   */
        gpio_clear(GPIOB, GPIO0);
        delay(20);                     /* debounce         */

        while (PRESSED);               /* wait for release */
        gpio_set(GPIOB, GPIO0);
        delay(20);
    }
}
