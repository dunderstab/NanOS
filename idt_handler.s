.extern irq_handler_next
.globl irq_handler

irq_handler:
    call irq_handler_next
    iret
