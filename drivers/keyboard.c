#include "../include/types.h"
#define PS2D 0x60
#define PS2S 0x64
static const char scn[128]={0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const char scs[128]={0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#define KB 512
static char kb[KB];
static uint32_t kh=0,kt=0;
static bool sh=false,cp=false;
static bool empty(void){return kh==kt;}
static bool full(void){return((kh+1)%KB)==kt;}
static void push(char c){if(!full()){kb[kh]=c;kh=(kh+1)%KB;}}
static char pop(void){if(empty())return 0;char c=kb[kt];kt=(kt+1)%KB;return c;}
void keyboard_init(void){kh=0;kt=0;sh=false;cp=false;while(inb(PS2S)&1)inb(PS2D);}
static void poll(void){
    while(inb(PS2S)&1){
        uint8_t s=inb(PS2D);
        if(s&0x80){uint8_t r=s&0x7F;if(r==0x2A||r==0x36)sh=false;continue;}
        if(s==0x2A||s==0x36){sh=true;continue;}
        if(s==0x3A){cp=!cp;continue;}
        if(s<128){char c=sh?scs[s]:scn[s];if(cp&&c>='a'&&c<='z')c-=32;else if(cp&&c>='A'&&c<='Z'&&!sh)c+=32;if(c)push(c);}
    }
}
bool keyboard_haschar(void){poll();return!empty();}
char keyboard_getchar(void){while(!keyboard_haschar())__asm__ volatile("pause");return pop();}
void keyboard_flush(void){kh=kt=0;}
