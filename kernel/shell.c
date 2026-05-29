#include "../include/types.h"

extern void vga_puts(const char*s);
extern void vga_puts_col(const char*s,uint8_t f,uint8_t b);
extern void vga_putchar(char c);
extern void vga_puthex(uint32_t n);
extern void vga_putdec(uint32_t n);
extern void vga_clear(void);
extern void vga_setcolor(uint8_t f,uint8_t b);
extern void vga_init(void);
extern char keyboard_getchar(void);
extern bool keyboard_haschar(void);
extern void keyboard_flush(void);
extern int  bf_exec(const char*prog,uint32_t len);

#define IBUF 4096
static char ibuf[IBUF];
static uint32_t ilen=0;

static uint32_t kstrlen(const char*s){uint32_t n=0;while(s[n])n++;return n;}
static int kstrcmp(const char*a,const char*b){while(*a&&*a==*b){a++;b++;}return (uint8_t)*a-(uint8_t)*b;}
static void kstrcpy(char*d,const char*s){while((*d++=*s++));}

static const char *BUILTIN_HELLO=
"++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

static const char *BUILTIN_FIB=
">++++++++++>+>+[[+++++[>++++++++<-]>.<++++++[>--------<-]+<<<]>.>>[[-]<[>+<-]>>[<<+>+>-]<[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>+<-[>[-]>+>+<<<-[>+<-]]]]]]]]]]]+>>>]<<<]";

static const char *BUILTIN_ROT13=
"-,+[-[>>++++[>++++++++<-]<+<-[>+>[->++[-[>>++++[>++++++++<-]<]<[>>[-<]<[->>]<<]>>]>[>--<-[-<<<+>>>]]<[>>+<<]>]]]>[-]>>[-]+<[>><-[>+<-]]<<.[-]<]";

static void readline(void){
    ilen=0;
    while(1){
        char c=keyboard_getchar();
        if(c=='\n'||c=='\r'){vga_putchar('\n');break;}
        if(c=='\b'){if(ilen>0){ilen--;vga_putchar('\b');}}
        else if(ilen<IBUF-1){ibuf[ilen++]=c;vga_putchar(c);}
    }
    ibuf[ilen]=0;
}

static void run_bf(const char*prog,uint32_t len){
    vga_puts_col("\n[BF RUN]\n",10,0);
    int r=bf_exec(prog,len);
    if(r==0) vga_puts_col("\n[OK]\n",10,0);
    else{vga_puts_col("\n[ERR:",12,0);vga_putdec(r);vga_puts_col("]\n",12,0);}
}

static void cmd_help(void){
    vga_puts_col("Legion BF OS Commands:\n",14,0);
    vga_puts_col("  help      - show this\n",7,0);
    vga_puts_col("  clear     - clear screen\n",7,0);
    vga_puts_col("  hello     - BF hello world\n",7,0);
    vga_puts_col("  fib       - BF fibonacci\n",7,0);
    vga_puts_col("  rot13     - BF rot13 encoder\n",7,0);
    vga_puts_col("  run       - run BF code interactively\n",7,0);
    vga_puts_col("  about     - about Legion BF OS\n",7,0);
    vga_puts_col("  reboot    - reboot machine\n",7,0);
}

static void cmd_about(void){
    vga_puts_col("\n Legion Brainfuck OS\n",11,0);
    vga_puts_col(" Demo X Hexa Edition\n",13,0);
    vga_puts_col(" By DeathLegionTeamLK\n",9,0);
    vga_puts_col(" Fully Free & Open Source\n",14,0);
    vga_puts_col(" Brainfuck IS the shell language\n",7,0);
    vga_puts_col(" Real x86 kernel, no simulator\n\n",7,0);
}

static void cmd_run(void){
    vga_puts_col("Enter BF code (Enter to run):\n> ",11,0);
    readline();
    if(ilen>0) run_bf(ibuf,ilen);
}

static void cmd_reboot(void){
    outb(0x64,0xFE);
    while(1)__asm__ volatile("hlt");
}

void shell_run(void){
    while(1){
        vga_puts_col("\nbf> ",11,0);
        readline();
        if(ilen==0) continue;
        if(kstrcmp(ibuf,"help")==0)    cmd_help();
        else if(kstrcmp(ibuf,"clear")==0)  vga_clear();
        else if(kstrcmp(ibuf,"hello")==0)  run_bf(BUILTIN_HELLO,kstrlen(BUILTIN_HELLO));
        else if(kstrcmp(ibuf,"fib")==0)    run_bf(BUILTIN_FIB,kstrlen(BUILTIN_FIB));
        else if(kstrcmp(ibuf,"rot13")==0)  run_bf(BUILTIN_ROT13,kstrlen(BUILTIN_ROT13));
        else if(kstrcmp(ibuf,"run")==0)    cmd_run();
        else if(kstrcmp(ibuf,"about")==0)  cmd_about();
        else if(kstrcmp(ibuf,"reboot")==0) cmd_reboot();
        else{
            bool is_bf=true;
            for(uint32_t i=0;i<ilen;i++){
                char c=ibuf[i];
                if(c!='>'&&c!='<'&&c!='+'&&c!='-'&&c!='.'&&c!=','&&c!='['&&c!=']'&&c!=' '&&c!='\t'){is_bf=false;break;}
            }
            if(is_bf&&ilen>0) run_bf(ibuf,ilen);
            else{vga_puts_col("Unknown: ",12,0);vga_puts(ibuf);vga_putchar('\n');}
        }
    }
}
