#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Number of entries in the IDT
#define IDT_ENTRIES 256

// Struct for IDT entry
struct idt_entry_t {
    uint16_t base_lo;  // Lower 16 bits of handler function address
    uint16_t sel;      // Kernel segment selector
    uint8_t always0;   // Must always be zero
    uint8_t flags;     // Flags for entry (present, privilege level, type)
    uint16_t base_hi;  // Upper 16 bits of handler function address
} __attribute__((packed));

// Struct for IDT pointer
struct idt_ptr_t {
    uint16_t limit;    // Size of IDT
    uint32_t base;     // Base address of IDT
} __attribute__((packed));

// Define macros for setting flags in IDT entries
#define IDT_PRESENT_BIT 0x80
#define IDT_RING0 0x00
#define IDT_RING3 0x60
#define IDT_INT_GATE 0x0E
#define IDT_TRAP_GATE 0x0F
#define IDT_SIZE 0x08

// Define an IRQ handler function
extern void irq_handler();
extern void irq_handler_next();

// Define an ISR handler function
void isr_handler();

// Define a function to load the IDT
void idt_load();

// Define a function to initialize the IDT
void idt_init(void);

// Define a function to set a gate in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif // IDT_H
