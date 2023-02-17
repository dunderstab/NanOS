

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Library Componenets
#include "lib/stdio.h"

// Necessary Kernel Components
#include "gdt.h"
#include "idt.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You not using a cross-compiler to compile the OS from source, you will most certainly run into trouble. If this step is necessary, please refer to your platform's C compiler documentation for help and use Freestanding mode in compilation."
#endif
 
/* will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "Source needs to be compiled with a ix86-elf compiler"
#endif
 
#include <stdarg.h>





void kernel_main(void) 
{	
	clear_screen();
	nanos_printf("Hello NanOS!\n");
	gdt_init();
	nanos_printf("Initialized Global Descriptor Table.\n");
	idt_init();
	nanos_printf("Initialized Interrupt Descriptor Table.\n");

	while (true) {}
}