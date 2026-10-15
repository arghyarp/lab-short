/* S1 Q9: Q7 with ON/OFF = 40/60, 55/60, 55/55, 35/65, 90/1 (1 unit = 10 ms).
 * Each pattern runs 5 times, then the next one. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 8000; i++); }

int main(void)
{
    int on[]  = {40, 55, 55, 35, 90};
    int off[] = {60, 60, 55, 65,  1};

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

    while (1) {
        for (int p = 0; p < 5; p++) {
            for (int r = 0; r < 5; r++) {
                gpio_set(GPIOB, GPIO0);   delay(on[p] * 10);
                gpio_clear(GPIOB, GPIO0); delay(off[p] * 10);
            }
        }
    }
}
