# STM32F103C8T6 / libopencm3 / ST-Link
.DEFAULT_GOAL := all
PREFIX ?= arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size
STFLASH ?= st-flash
ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
OPENCM3_DIR ?= $(ROOT)libopencm3
LDSCRIPT ?= $(ROOT)stm32f103c8t6.ld
LIB := $(OPENCM3_DIR)/lib/libopencm3_stm32f1.a
ARCH := -mcpu=cortex-m3 -mthumb -msoft-float
CFLAGS := $(ARCH) -Os -g -std=c99 -Wall -Wextra -DSTM32F1 -I$(OPENCM3_DIR)/include -ffunction-sections -fdata-sections
LDFLAGS := $(ARCH) -nostartfiles --specs=nano.specs --specs=nosys.specs -L$(OPENCM3_DIR)/lib -L$(OPENCM3_DIR)/lib/stm32/f1 -T$(LDSCRIPT) -Wl,--gc-sections
# Each C file is a separate lab program, not a module of a larger program.
ifeq ($(abspath $(CURDIR)),$(abspath $(ROOT)))
SRCS := $(wildcard */*.c)
else
SRCS := $(wildcard *.c)
endif
ELFS := $(SRCS:.c=.elf)
BINS := $(SRCS:.c=.bin)
P ?= $(if $(filter 1,$(words $(SRCS))),$(basename $(SRCS)))

.PHONY: all elf bin flash clean clobber
all: $(BINS)
elf: $(ELFS)
bin: $(BINS)

%.elf: %.c $(LDSCRIPT) $(ROOT)Makefile $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -lopencm3_stm32f1 -o $@
	$(SIZE) $@

$(LIB):
	@echo 'Missing libopencm3 STM32F1 library. Build it with:' >&2
	@echo 'make -C $(OPENCM3_DIR) TARGETS=stm32/f1' >&2
	@exit 1

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

ifneq ($(strip $(P)),)
flash: $(P).bin
	$(STFLASH) --reset write $(P).bin 0x08000000
else
flash:
	@echo 'Select a program: make flash P=path/to/program (without .c)' >&2
	@echo 'Available programs: $(basename $(SRCS))' >&2
	@exit 1
endif

clean clobber:
	rm -f $(ELFS) $(BINS) $(SRCS:.c=.o)

.PRECIOUS: %.elf
