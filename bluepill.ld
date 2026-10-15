/* Linker script for STM32F103C8T6 (Blue Pill).
 * Tells the linker where Flash and RAM are; libopencm3's generic
 * script does the rest (vector table, .text, .data, .bss, stack). */
MEMORY
{
    rom (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

INCLUDE cortex-m-generic.ld
