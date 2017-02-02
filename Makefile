OBJCOPY = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld

INCLUDE_DIR="-I$(shell pwd)"
CFLAGS=-O2 -g -Wall $(INCLUDE_DIR)
LDFLAGS=-lgcc -nostartfiles

all : kernel

OBJ = printf.o \
	  kernel.o \
      boot/boot.o \
	  drivers/uart/uart.o

SRC = $(OBJ:%.o=%.c)

boot/%.o:boot/%.s
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name "*.o" | xargs rm -f
	rm -f kernel.elf
	rm -f kernel.img

kernel: $(OBJ)
	$(CC) $(LDFLAGS) -T linker.ld -o kernel.elf $(OBJ)
	$(OBJCOPY) kernel.elf -O binary kernel.img
