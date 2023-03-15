

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Library Componenets
#include "lib/stdio.h"

// Include Components
#include "include/io.h"

// Necessary Kernel Components
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

// Memory Management Components
#include "kheap.h"
#include "paging.h"

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

	// Initialise all the ISRs and segmentation
	gdt_init();
	idt_init();

	
	nanos_printf("Hello, paging world!\n");
	uint32_t a = kmalloc(8);
	initialize_paging();
	uint32_t b = kmalloc(8);
	uint32_t c = kmalloc(8);
	nanos_printf("a: %h, b: %h\n", a, b);
	nanos_printf("c: %h", c);

	kfree(c);
	kfree(b);
	uint32_t d = kmalloc(12);
	nanos_printf(", d: %h\n", d);

	for (;;);
	
}