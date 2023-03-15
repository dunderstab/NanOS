#include "keyboard.h"
#include "include/io.h"
#include "lib/stdio.h"
#include "idt.h"

#include <stdint.h>


// The keyboard controller's IO port addresses
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_COMMAND_PORT 0x64

// The keyboard controller commands
#define COMMAND_READ_MODE 0x20
#define COMMAND_WRITE_MODE 0x60
#define MODE_INTERRUPT_ENABLED 0x01

#define SHIFT_OFFSET 0x80

static unsigned int keyboard_buffer_index = 0;

extern isr_t interrupt_handlers[256];

void initialize_keyboard() {
    nanos_printf("Loading keyboard...\n");
    interrupt_handlers[33] = keyboard_interrupt_handler;
    nanos_printf("Loading keyboard...\n");

    return;
}

void keyboard_interrupt_handler(void) {
    unsigned char s = inb(0x64);
    if (!(s & 1)) {
        return;
    }
    unsigned char x = inb(0x60);
    if (x == 159) {
        return;
    }
    unsigned char j = scancode_to_ascii(x);
    keyboard_buffer[keyboard_buffer_index] = j;

    keyboard_buffer_index++;
    
}

unsigned char read_stdin() {
    if (keyboard_buffer_index != 0) {
        unsigned char c = keyboard_buffer[keyboard_buffer_index - 1];
        keyboard_buffer[keyboard_buffer_index] = 0;
        keyboard_buffer_index--;
        return c;
    } else { return 0; }
}

unsigned char scancode_to_ascii(unsigned char scancode) {
    switch(scancode) {
        case 0x01: return 27; // Escape
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '-';
        case 0x0D: return '=';
        case 0x0E: return '\b'; // Backspace
        case 0x0F: return '\t'; // Tab
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1A: return '[';
        case 0x1B: return ']';
        case 0x1C: return '\n';
        case 0x1D: return 0; // Left Control
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2A: return 0; // Left Shift
        case 0x2B: return '\\';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x36: return 0; // Right Shift
        case 0x37: return '*';
        case 0x38: return 0; // Left Alt
        case 0x39: return ' ';
        case 0x2A + SHIFT_OFFSET:  return 0; // Right Shift (shifted)
        case 0x27 + SHIFT_OFFSET:  return ':';
        case 0x28 + SHIFT_OFFSET:  return '"';
        case 0x29 + SHIFT_OFFSET:  return '~';
        case 0x2B + SHIFT_OFFSET:  return '|';
        default: return 0; // Unknown scancode
    }
}
