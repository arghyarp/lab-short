/* S1 Q3b: 3 LEDs (PB0, PB1, PB10) - all together, then one by one. */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delay(int ms) { for (volatile int i = 0; i < ms * 7000; i++); }

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO0 | GPIO1 | GPIO10);     /* | = several pins in one call */

    while (1) {
        gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO10);   /* all on  */
        delay(1000);
        gpio_clear(GPIOB, GPIO0 | GPIO1 | GPIO10); /* all off */
        delay(1000);

        gpio_set(GPIOB, GPIO0);  delay(300);  gpio_clear(GPIOB, GPIO0);
        gpio_set(GPIOB, GPIO1);  delay(300);  gpio_clear(GPIOB, GPIO1);
        gpio_set(GPIOB, GPIO10); delay(300);  gpio_clear(GPIOB, GPIO10);
	delay(1000);
    }
}
