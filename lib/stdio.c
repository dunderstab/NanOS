#include "math.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void clear_screen() {
    char* video_memory = (char*) 0xb8000;
    int i;
    for (i = 0; i < 80*25*2; i++) {
        *(video_memory + i) = 0;
    }
}

void print_char(char c) {
    uint16_t* video_memory = (uint16_t*) 0xb8000;
    static uint8_t x = 0, y = 0;
    uint8_t attribute_byte = (0 /* black */ << 4) | (15 /* white */ & 0x0f);
    uint16_t attribute = attribute_byte << 8;
    if (c == 0x08 && x) {
        x--;
    } else if (c == '\r') {
        x = 0;
    } else if (c == '\n') {
        x = 0;
        y++;
    } else if (c == '\b') {

        if (x == 0)
        {   
            y--;
            x = 80;
        } else {
            x--;   
        }
        video_memory[(y * 80) + x] = 0;
    } else if (c >= ' ') {
        video_memory[(y * 80) + x] = c | attribute;
        x++;
    }
    if (x >= 80) {
        x = 0;
        y++;
    }
    if (y >= 25) {
        for (y = 0; y < 25; y++) {
            for (x = 0; x < 80; x++) {
                video_memory[(y * 80) + x] = video_memory[((y + 1) * 80) + x];
            }
        }
        for (x = 0; x < 80; x++) {
            video_memory[(24 * 80) + x] = ' ' | attribute;
        }
        x = 0;
        y = 24;
    }
}


void print_string(char* str) {
    int i = 0;
    while (str[i]) {
        print_char(str[i]);
        i++;
    }
}

void print_int(int num) {
    char buffer[20];
    int i = 0;
    if (num < 0) {
        print_char('-');
        num = -num;
    }
    if (num == 0) {
        print_char('0');
    } else {
        while (num != 0) {
            buffer[i++] = '0' + num % 10;
            num /= 10;
        }
        while (i > 0) {
            print_char(buffer[--i]);
        }
    }
}
void nanos_printf(char* format, ...) {
    va_list args;
    va_start(args, format);
    int i = 0;
    while (format[i]) {
        if (format[i] == '%') {
            i++;
            if (format[i] == 's') {
                char* str = va_arg(args, char*);
                print_string(str);
            } else if (format[i] == 'd') {
                int num = va_arg(args, int);
                print_int(num);
            } else if (format[i] == 'c') {
                char c = va_arg(args, char);
                print_char(c);
            } else {
                print_char('%');
                print_char(format[i]);
            }
        } else if (format[i] == '\n') {
            print_char(format[i]);
        } else {
            print_char(format[i]);
        }
        i++;
    }
    va_end(args);
}