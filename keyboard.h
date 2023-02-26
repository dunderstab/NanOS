#ifndef __KEYBOARD_H
#define __KEYBOARD_H

unsigned char scancode_to_ascii(unsigned char scancode);
void initialize_keyboard();


static char keyboard_buffer[256];
static unsigned int keyboard_buffer_index;
void keyboard_interrupt_handler(unsigned char scancode);

unsigned char read_stdin();

#endif