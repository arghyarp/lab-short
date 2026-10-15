# Builds every lab program into its own .elf and .bin
#
#   make                         build all
#   make OPENCM3_DIR=/path/to/libopencm3
#   make flash P=set1/s1_q3a_blink     flash one program with st-flash
#
# Needs: arm-none-eabi-gcc, libopencm3 built with `make TARGETS=stm32/f1`

#OPENCM3_DIR ?= ../libopencm3
OPENCM3_DIR ?= /home/arghya/MCA/stm32/stm32f103c8t6/libopencm3
PREFIX      ?= arm-none-eabi-

CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE    = $(PREFIX)size

ARCH    = -mcpu=cortex-m3 -mthumb -msoft-float
CFLAGS  = $(ARCH) -Os -g -std=c99 -Wall -Wextra -DSTM32F1 \
          -I$(OPENCM3_DIR)/include -ffunction-sections -fdata-sections
LDFLAGS = $(ARCH) -nostartfiles --specs=nano.specs --specs=nosys.specs \
          -Tbluepill.ld -L$(OPENCM3_DIR)/lib -Wl,--gc-sections
LDLIBS  = -lopencm3_stm32f1

SRCS = $(wildcard set1/*.c set2/*.c adc/*.c)
ELFS = $(SRCS:.c=.elf)
BINS = $(SRCS:.c=.bin)

all: $(BINS)

%.elf: %.c bluepill.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@
	@$(SIZE) $@ | tail -1

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

flash: $(P).bin
	st-flash --reset write $(P).bin 0x08000000

clean:
	rm -f */*.elf */*.bin

.PHONY: all flash clean
.PRECIOUS: %.elf
