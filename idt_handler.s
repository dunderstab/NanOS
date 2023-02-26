.extern irq_handler_next
.extern isr_handler_next
.globl isr_handler
.globl irq_handler

irq_handler:
    pusha                 # Push all general purpose registers
    cld                    # Clear direction flag (forward movement)
    call irq_handler_next
    popa                  # Restore all general purpose registers
    iret                  # IRET will restore required parts of EFLAGS
                           #   including the direction flag
isr_handler:
    call isr_handler_next
    iret                  # IRET will restore required parts of EFLAGS
                           #   including the direction flag


