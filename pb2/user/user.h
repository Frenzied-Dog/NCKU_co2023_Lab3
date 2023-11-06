#ifndef __USER__
#define __USER__

#include <stdint.h>
#include <stdarg.h>

extern int32_t divide(int32_t, int32_t);  // divide operation
extern int32_t mod(int32_t, int32_t);     // modulo operation

extern void call_to_emu(int32_t, int32_t) __attribute__((optimize("O0"))); // defined in call_to_emu.S
extern void exit(void);
extern void putchar(char);
extern void puts(char*);
extern void putint(int32_t);
extern void puthex_L(uint32_t);
extern void puthex_U(uint32_t);
extern void printf(char* format, ...);

#endif
