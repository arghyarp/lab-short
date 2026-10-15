/* S1 Q3a: blink LED on PB0.  Wiring: PB0 -> 330R -> LED -> GND */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

    while (1) {
        gpio_toggle(GPIOB, GPIO0);
        delay(500);
    }
}
