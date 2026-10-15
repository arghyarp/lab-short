/* S1 Q1: on-board LED is on PC13, wired 3.3V -> LED -> PC13, so it is ACTIVE-LOW:
 *        gpio_clear = ON, gpio_set = OFF. (PWR LED is always on, not controllable.)
 * Avoid using: PA9/PA10 (UART), PA11/PA12 (USB), PA13/PA14 (programmer). */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    while (1) {
        gpio_clear(GPIOC, GPIO13);   /* ON  */
        delay(500);
        gpio_set(GPIOC, GPIO13);     /* OFF */
        delay(500);
    }
}
