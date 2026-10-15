/* S1 Q7: LED (PB0) ON 70 %, OFF 30 % of a 1000 ms period (duty cycle 70 %). */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 8000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);

    while (1) {
        gpio_set(GPIOB, GPIO0);   delay(700);
        gpio_clear(GPIOB, GPIO0); delay(300);
    }
}
