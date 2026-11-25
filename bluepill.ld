/* STM32F103C8T6: 64 KiB flash, 20 KiB SRAM. */
MEMORY
{
    rom (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}
INCLUDE cortex-m-generic.ld
