#ifndef TYPES_H
#define TYPES_H
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed int         int32_t;
typedef unsigned int       size_t;
typedef uint8_t            bool;
#define true  1
#define false 0
#define NULL  ((void*)0)
static inline void outb(uint16_t port, uint8_t val){__asm__ volatile("outb %0,%1"::"a"(val),"Nd"(port));}
static inline uint8_t inb(uint16_t port){uint8_t r;__asm__ volatile("inb %1,%0":"=a"(r):"Nd"(port));return r;}
#endif
