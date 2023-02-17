#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

void clear_screen();
void print_char(char c);
void print_string(char* str);
void print_int(int num);
void nanos_printf(char* format, ...);


#endif