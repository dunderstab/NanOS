#ifndef _IO_H
#define _IO_H

#define PIC1_DATA           0x21
#define PIC2_DATA           0xA1
#define PIC1_COMMAND        0x20
#define PIC2_COMMAND        0xA0

#define PIC1_IRQ_BASE       0x20
#define PIC2_IRQ_BASE       0x28

#define ICW1_INIT           0x10
#define ICW1_ICW4           0x01
#define ICW4_8086           0x01

#include <stdint.h>

// Define input/output functions for x86 architecture

static inline uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %w1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %w1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %w1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %w1" : : "a"(data), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t data)
{
    asm volatile("outw %0, %w1" : : "a"(data), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t data)
{
    asm volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}

void io_wait(void) {
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    __asm__ __volatile__("outb %%al, $0x80" : : "a"(0));
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

// INIT PICs
void init_pic()
{
    // Save the masks for later
    uint8_t mask_master = inb(PIC1_DATA);
    uint8_t mask_slave = inb(PIC2_DATA);
    
    // Start the initialization sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // Set the IRQ base offsets
    outb(PIC1_DATA, PIC1_IRQ_BASE);
    io_wait();
    outb(PIC2_DATA, PIC2_IRQ_BASE);
    io_wait();

    // Tell the master PIC that there is a slave PIC on IRQ line 2
    outb(PIC1_DATA, 1 << 2);
    io_wait();

    // Tell the slave PIC to use IRQ line 2
    outb(PIC2_DATA, 2);
    io_wait();

    // Set the mode (8086 mode) and restore the masks
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    outb(PIC1_DATA, mask_master);
    outb(PIC2_DATA, mask_slave);
}


#endif /* _IO_H */
