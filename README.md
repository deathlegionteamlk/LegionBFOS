# Legion Brainfuck OS — Demo X Hexa Edition
By DeathLegionTeamLK | 100% Free | Real x86 Kernel

## What This Is
A real bootable x86 OS kernel where Brainfuck is the shell language.
Not a simulator. Not a web demo. A real ELF32 binary that boots on x86 hardware via GRUB Multiboot.

## Build Requirements
- gcc with multilib (gcc-multilib)
- binutils (ld, as, objcopy)
- grub-pc-bin + grub-common + xorriso (for ISO)
- Any x86 Linux system

## Build Steps
```
sudo apt install gcc-multilib grub-pc-bin grub-common xorriso
make
```

## Run in QEMU
```
qemu-system-i386 -cdrom legion.iso
```

## Shell Commands
| Command | What it does |
|---------|-------------|
| help    | List commands |
| clear   | Clear screen |
| hello   | Run BF hello world |
| fib     | Run BF fibonacci |
| rot13   | Run BF ROT13 encoder |
| run     | Enter and run BF code |
| about   | About Legion BF OS |
| reboot  | Reboot machine |

You can also type raw Brainfuck directly at the prompt and it runs instantly.

## Architecture
- boot/boot.S       — Multiboot header + x86 entry point
- boot/linker.ld    — Kernel memory layout (loads at 1MB)
- drivers/vga.c     — Real VGA text mode (writes to 0xB8000)
- drivers/keyboard.c— Real PS/2 keyboard (polls port 0x60)
- kernel/bf.c       — Brainfuck interpreter (64KB tape)
- kernel/shell.c    — Interactive BF shell with built-in programs
- kernel/main.c     — Kernel entry, banner, boot sequence

## License
Free. Do whatever you want with it.
