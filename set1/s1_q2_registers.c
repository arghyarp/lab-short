/* S1 Q2: blink PC13 with NO library - this is what the header files hide.
 *
 * Memory map: Flash 0x08000000 | RAM 0x20000000 | Peripherals 0x40000000
 *   RCC   = 0x40021000   APB2ENR at +0x18 (bit 4 = GPIOC clock)
 *   GPIOC = 0x40011000   CRH at +0x04 (config pins 8-15, 4 bits each)
 *                        ODR at +0x0C (output value of each pin)
 * PC13 config = bits 20-23 of CRH. Value 0x2 = output 2 MHz push-pull.
 * volatile = "really read/write this address every time, don't optimise it away". */
#define RCC_APB2ENR (*(volatile unsigned int *)0x40021018)
#define GPIOC_CRH   (*(volatile unsigned int *)0x40011004)
#define GPIOC_ODR   (*(volatile unsigned int *)0x4001100C)

int main(void)
{
    RCC_APB2ENR |= (1 << 4);           /* GPIOC clock on      */
    GPIOC_CRH &= ~(0xF << 20);         /* clear PC13 config   */
    GPIOC_CRH |=  (0x2 << 20);         /* PC13 = output       */

    while (1) {
        GPIOC_ODR ^= (1 << 13);        /* toggle PC13         */
        for (volatile int i = 0; i < 500000; i++);
    }
}
