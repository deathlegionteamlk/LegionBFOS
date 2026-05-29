#include "../include/types.h"
#define VGA ((volatile uint16_t*)0xB8000)
#define W 80
#define H 25
#define CTRL 0x3D4
#define DATA 0x3D5
static uint8_t row=0,col=0,fg=11,bg=0;
static uint16_t mk(char c,uint8_t f,uint8_t b){return((uint16_t)((b<<4)|(f&0xF))<<8)|(uint8_t)c;}
static void cur(void){uint16_t p=row*W+col;outb(CTRL,0x0F);outb(DATA,p&0xFF);outb(CTRL,0x0E);outb(DATA,(p>>8)&0xFF);}
static void scr(void){
    for(uint32_t r=1;r<H;r++)for(uint32_t c2=0;c2<W;c2++)VGA[(r-1)*W+c2]=VGA[r*W+c2];
    for(uint32_t c2=0;c2<W;c2++)VGA[(H-1)*W+c2]=mk(' ',fg,bg);
    if(row>0)row--;
}
void vga_init(void){fg=11;bg=0;for(uint32_t i=0;i<W*H;i++)VGA[i]=mk(' ',fg,bg);row=0;col=0;cur();outb(CTRL,0x0A);outb(DATA,(inb(DATA)&0xC0)|14);outb(CTRL,0x0B);outb(DATA,(inb(DATA)&0xE0)|15);}
void vga_setcolor(uint8_t f,uint8_t b){fg=f;bg=b;}
void vga_putchar(char c){
    if(c=='\n'){col=0;if(++row>=H)scr();}
    else if(c=='\r'){col=0;}
    else if(c=='\b'){if(col>0){col--;VGA[row*W+col]=mk(' ',fg,bg);}}
    else{VGA[row*W+col]=mk(c,fg,bg);if(++col>=W){col=0;if(++row>=H)scr();}}
    cur();
}
void vga_puts(const char*s){while(*s)vga_putchar(*s++);}
void vga_puts_col(const char*s,uint8_t f,uint8_t b){uint8_t of=fg,ob=bg;fg=f;bg=b;vga_puts(s);fg=of;bg=ob;}
void vga_puthex(uint32_t n){const char*h="0123456789ABCDEF";vga_puts("0x");for(int i=7;i>=0;i--)vga_putchar(h[(n>>(i*4))&0xF]);}
void vga_putdec(uint32_t n){if(!n){vga_putchar('0');return;}char b[12];int i=0;while(n){b[i++]='0'+(n%10);n/=10;}for(int j=i-1;j>=0;j--)vga_putchar(b[j]);}
void vga_clear(void){for(uint32_t i=0;i<W*H;i++)VGA[i]=mk(' ',fg,bg);row=0;col=0;cur();}
uint8_t vga_get_row(void){return row;}
uint8_t vga_get_col(void){return col;}
