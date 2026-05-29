#include "../include/types.h"

extern void vga_init(void);
extern void vga_puts(const char*s);
extern void vga_puts_col(const char*s,uint8_t f,uint8_t b);
extern void vga_putchar(char c);
extern void vga_clear(void);
extern void vga_setcolor(uint8_t f,uint8_t b);
extern void keyboard_init(void);
extern void shell_run(void);

static void draw_banner(void){
    vga_clear();
    vga_puts_col("##############################################################\n",9,0);
    vga_puts_col("#                                                            #\n",9,0);
    vga_puts_col("#        LEGION BRAINFUCK OS  -  Demo X Hexa Edition        #\n",11,0);
    vga_puts_col("#               By DeathLegionTeamLK                        #\n",13,0);
    vga_puts_col("#            Real x86 Kernel  |  100% Free                  #\n",14,0);
    vga_puts_col("#    Brainfuck IS the shell  |  No simulator  |  No fake    #\n",7,0);
    vga_puts_col("#                                                            #\n",9,0);
    vga_puts_col("##############################################################\n",9,0);
    vga_puts_col("\nType 'help' for commands. Type BF code directly to run it.\n\n",10,0);
}

void kmain(uint32_t magic, uint32_t mb_info){
    (void)magic;
    (void)mb_info;
    vga_init();
    keyboard_init();
    draw_banner();
    shell_run();
    while(1)__asm__ volatile("cli; hlt");
}
