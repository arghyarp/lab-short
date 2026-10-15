/* S1 Q8: go through a series; LED (PB0) on for odd numbers, off for even.
 * Odd test: n % 2 == 1. Short 100 ms gap shows where each number starts. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 6000; i++); }

int main(void)
{
    int nums[] = {3, 8, 5, 7, 2, 9, 4, 1};

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

    while (1) {
        for (int i = 0; i < 8; i++) {
            if (nums[i] % 2 == 1) gpio_set(GPIOB, GPIO0);
            else                  gpio_clear(GPIOB, GPIO0);
            delay(1000);
            gpio_clear(GPIOB, GPIO0);
            delay(100);
        }
    }
}
