.PHONY: all clean

all: run clean

run: nanos.bin
	qemu-system-i386 -kernel nanos.bin

nanos.bin: boot.o kernel.o math.o stdio.o gdt.o idt.o idt_handler.o keyboard.o
	i686-elf-gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

%.o: %.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: lib/%.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: kernel/%.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: %.s
	i686-elf-as $< -o $@

boot.o: boot/boot.s
	i686-elf-as boot/boot.s -o boot.o

clean:
	rm -f *.o nanos.bin

