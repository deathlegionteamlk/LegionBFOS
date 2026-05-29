#!/bin/bash
set -e
echo "[1/6] Installing dependencies..."
sudo apt install -y gcc-multilib binutils grub-pc-bin grub-common xorriso 2>/dev/null

echo "[2/6] Compiling VGA driver..."
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c drivers/vga.c -o drivers/vga.o

echo "[3/6] Compiling keyboard driver..."
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c drivers/keyboard.c -o drivers/keyboard.o

echo "[4/6] Compiling Brainfuck kernel..."
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c kernel/bf.c -o kernel/bf.o
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c kernel/shell.c -o kernel/shell.o
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c kernel/main.c -o kernel/main.o

echo "[5/6] Assembling bootloader..."
gcc -m32 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -fno-builtin -O2 -Iinclude -c boot/boot.S -o boot/boot.o

echo "[6/6] Linking kernel & building ISO..."
ld -m elf_i386 -T boot/linker.ld --oformat elf32-i386 \
   boot/boot.o drivers/vga.o drivers/keyboard.o kernel/bf.o kernel/shell.o kernel/main.o \
   -o kernel.elf
cp kernel.elf iso/boot/kernel.elf
grub-mkrescue -o legion.iso iso

echo ""
echo "======================================"
echo "  Legion Brainfuck OS — BUILT!"
echo "  legion.iso is ready"
echo "  Run: qemu-system-i386 -cdrom legion.iso"
echo "======================================"
