#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <stdint.h>

void clear_screen();
void print_char(char c);
void print_string(char* str);
void print_int(int num);
void nanos_printf(char* format, ...);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);


#endif