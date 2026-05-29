#include "../include/types.h"

#define TAPE_SIZE  65536
#define STACK_SIZE 1024

extern void vga_putchar(char c);
extern char keyboard_getchar(void);

static uint8_t tape[TAPE_SIZE];
static uint32_t bstack[STACK_SIZE];

static void tape_clear(void){for(uint32_t i=0;i<TAPE_SIZE;i++)tape[i]=0;}

int bf_exec(const char *prog, uint32_t len){
    tape_clear();
    uint32_t tp=0,pp=0,bsp=0;
    while(pp<len){
        char cmd=prog[pp];
        if(cmd=='>'){if(++tp>=TAPE_SIZE)return 1;}
        else if(cmd=='<'){if(tp==0)return 1;tp--;}
        else if(cmd=='+'){tape[tp]++;}
        else if(cmd=='-'){tape[tp]--;}
        else if(cmd=='.'){vga_putchar((char)tape[tp]);}
        else if(cmd==','){tape[tp]=(uint8_t)keyboard_getchar();}
        else if(cmd=='['){
            if(tape[tp]==0){
                uint32_t d=1;
                while(d>0&&++pp<len){if(prog[pp]=='[')d++;else if(prog[pp]==']')d--;}
            } else {
                if(bsp>=STACK_SIZE)return 2;
                bstack[bsp++]=pp;
            }
        }
        else if(cmd==']'){
            if(bsp==0)return 3;
            if(tape[tp])pp=bstack[bsp-1];
            else bsp--;
        }
        pp++;
    }
    return 0;
}
