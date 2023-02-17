
#include <stdint.h>
#include "idt.h"
#include "lib/stdio.h"
#include "include/io.h"

// Define the Interrupt Descriptor Table (IDT)
struct idt_entry_t idt[IDT_ENTRIES];
struct idt_entry_t idt_entries[IDT_ENTRIES];

// Load the IDT pointer
struct idt_ptr_t idt_ptr = {
    .limit = sizeof(idt) - 1,
    .base = (uint32_t)&idt
};

typedef struct registers {
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically
} registers_t;

// Define an IRQ handler function
void irq_handler_next(registers_t *regs)
{
    uint8_t irq = regs->int_no;
    if (irq >= PIC1_IRQ_BASE && irq < PIC1_IRQ_BASE + 8) {
        irq -= PIC1_IRQ_BASE;
    } else if (irq >= PIC2_IRQ_BASE && irq < PIC2_IRQ_BASE + 8) {
        irq -= PIC2_IRQ_BASE;
    }
    nanos_printf("Received IRQ %d!\n", irq);

    // Send an EOI (end-of-interrupt) signal to the PICs
    if (irq >= 8 && irq <= 15) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);

    __asm__ __volatile__("sti");
}


void isr_handler()
{
    nanos_printf("\n\nSTOP\n\nException occurred... halting...");
    // Halt the CPU
    for (;;);
}

void idt_load()
{
    asm volatile("lidt %0" : : "m"(idt_ptr));
}

void idt_init(void)
{   
    // Disable interrupts
    __asm__ __volatile__("cli");

    // Setup the PIC(s)
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0x21, 0x01);
    outb(0x21, 0xFB);
    outb(0xA1, 0xFF);

    // Print a message to indicate that IDT is being loaded
    nanos_printf("Loading IDT\n");

    // Initialize the IDT pointer
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Set each IDT entry to the default handler
    for (int i = 0; i < IDT_ENTRIES; i++)
    {
        idt_set_gate(i, (uint32_t)isr_handler, 0x08, 0x8E);
    }

    // Set the IRQ entries in the IDT
    idt_set_gate(32, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq_handler, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq_handler, 0x08, 0x8E);

    // Send initialization control word 1 and 2 to both PICs
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Initialization Control Word 1
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // Send initialization control word 3 to both PICs
    outb(PIC1_DATA, 0x20); // Initialization Control Word 3: IRQ 0-7 map to IDT entries 0x20-0x27
    io_wait();
    outb(PIC2_DATA, 0x28); // Initialization Control Word 3: IRQ 8-15 map to IDT entries 0x28-0x2F
    io_wait();

    // Send initialization control word 4 to both PICs
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Unmask IRQs
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    // Load IDT
    idt_load();

    // Enable interrupts
    __asm__ __volatile__("sti");

    nanos_printf("IDT loaded\n");
}




void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    // Set the base address
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    // Set the selector
    idt[num].sel = sel;

    // Set the always0 field
    idt[num].always0 = 0;

    // Set the flags
    idt[num].flags = flags;
}