
#include <stdint.h>
#include "idt.h"
#include "lib/stdio.h"
#include "include/io.h"
#include "keyboard.h"

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
    

    // Send an EOI (end-of-interrupt) signal to the PICs
    
    int irq_num = irq + 1;
    
    switch (irq_num)
    {
        case 1:
            unsigned char s = inb(0x64);
            if (!(s & 1)) {
                break;
            }
            unsigned char x = inb(0x60);
            if (x == 159) {
                break;
            }
            keyboard_interrupt_handler(x);
            break;
        default:
            nanos_printf("Received IRQ %d!\n", irq_num);
            break;
    }
    if (irq >= 8 && irq <= 15) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
    __asm__ __volatile__("sti");
    return;
}


void isr_handler_next(int isr_num)
{
    
    nanos_printf("\nISR Number: %d\n", isr_num);
    outb(PIC1_COMMAND, 0x20);
    return;
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
        if (i != 7)
        {
             idt_set_gate(i, (uint32_t)isr_handler, 0x08, 0x8E);
        }
    }

    idt_set_gate(33, (uint32_t)irq_handler, 0x08, 0x8E);

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
    

    // From @Michael_Petch on StackOverflow :D
    outb(PIC1_DATA, 4); io_wait(); outb(PIC2_DATA, 2); io_wait();

    
    // Send initialization control word 4 to both PICs
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

       // Enable keyboard interrupts
    outb(0x21, inb(0x21) & ~(1 << 1));

    // Disable the PIT
    outb(0x43, 0x30); // send the control word to channel 0
    outb(0x40, 0x00); // send the initial count value (LSB)
    outb(0x40, 0x00); // send the initial count value (MSB)
    outb(0x21, 0xFC); // mask IRQ 0 and IRQ 1 (disable the PIT)


    // Load IDT
    idt_load();

    // Enable interrupts
    

    nanos_printf("IDT loaded\n");
    return;
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