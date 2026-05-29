CC      = gcc
AS      = as
LD      = ld
OBJCOPY = objcopy

CFLAGS  = -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector \
          -fno-builtin -O2 -Wall -Wextra -Iinclude
LDFLAGS = -m elf_i386 -T boot/linker.ld --oformat elf32-i386

SRCS_C  = drivers/vga.c drivers/keyboard.c kernel/bf.c kernel/shell.c kernel/main.c
SRCS_S  = boot/boot.S

OBJS    = $(SRCS_C:.c=.o) $(SRCS_S:.S=.o)

all: legion.iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

legion.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o legion.iso iso 2>/dev/null || \
	  (echo "no grub-mkrescue; kernel.elf built ok"; ls -lh kernel.elf)

clean:
	rm -f $(OBJS) kernel.elf legion.iso iso/boot/kernel.elf

.PHONY: all clean
