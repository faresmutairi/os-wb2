AS := nasm
CC := gcc
LD := ld
QEMU := qemu-system-i386

ASFLAGS := -f elf32 -g -F dwarf
CFLAGS := -m32 -std=gnu99 -ffreestanding -O0 -Wall -Wextra \
	-fno-stack-protector -fno-pic -fno-pie -fno-asynchronous-unwind-tables
LDFLAGS := -m elf_i386 -T source/link.ld

OBJECTS := source/loader.o source/kernel.o drivers/framebuffer.o
ISO_DIR := build/iso

.DEFAULT_GOAL := all

.PHONY: all check-tools check-iso-tools check check-symbols clean \
	run-direct run-curses run-iso debug

all: check-tools kernel.elf

check-tools:
	@command -v $(AS) >/dev/null || { echo "Error: nasm is not installed"; exit 1; }
	@command -v $(CC) >/dev/null || { echo "Error: gcc is not installed"; exit 1; }
	@command -v $(LD) >/dev/null || { echo "Error: ld is not installed"; exit 1; }

check-iso-tools:
	@command -v grub-mkrescue >/dev/null || { echo "Error: grub-mkrescue is not installed"; exit 1; }
	@command -v xorriso >/dev/null || { echo "Error: xorriso is not installed; use 'make run-direct' or 'make run-curses' for testing"; exit 1; }

source/loader.o: source/loader.asm
	$(AS) $(ASFLAGS) $< -o $@

source/kernel.o: source/kernel.c drivers/framebuffer.h
	$(CC) $(CFLAGS) -c $< -o $@

drivers/framebuffer.o: drivers/framebuffer.c drivers/framebuffer.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJECTS) source/link.ld
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

check-symbols: kernel.elf
	@command -v nm >/dev/null || { echo "Error: nm is not installed"; exit 1; }
	@nm kernel.elf | grep -Eq "[[:space:]]sum_of_three$$" || { echo "FAIL: sum_of_three symbol missing"; exit 1; }
	@nm kernel.elf | grep -Eq "[[:space:]]multiply$$" || { echo "FAIL: multiply symbol missing"; exit 1; }
	@nm kernel.elf | grep -Eq "[[:space:]]square$$" || { echo "FAIL: square symbol missing"; exit 1; }
	@echo "PASS: all three C mathematics functions are present in kernel.elf"

check: kernel.elf check-symbols
	@command -v grub-file >/dev/null || { echo "Error: grub-file is not installed"; exit 1; }
	@grub-file --is-x86-multiboot kernel.elf && echo "PASS: kernel.elf is Multiboot compliant"

os.iso: check check-iso-tools boot/grub/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp kernel.elf $(ISO_DIR)/boot/kernel.elf
	cp boot/grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

run-direct: kernel.elf
	@command -v $(QEMU) >/dev/null || { echo "Error: qemu-system-i386 is not installed"; exit 1; }
	$(QEMU) -kernel kernel.elf -m 32

run-curses: kernel.elf
	@command -v $(QEMU) >/dev/null || { echo "Error: qemu-system-i386 is not installed"; exit 1; }
	$(QEMU) -kernel kernel.elf -m 32 -display curses

run-iso: os.iso
	@command -v $(QEMU) >/dev/null || { echo "Error: qemu-system-i386 is not installed"; exit 1; }
	$(QEMU) -cdrom os.iso -m 32

debug: kernel.elf
	@command -v $(QEMU) >/dev/null || { echo "Error: qemu-system-i386 is not installed"; exit 1; }
	$(QEMU) -kernel kernel.elf -m 32 -s -S

clean:
	rm -rf source/*.o drivers/*.o kernel.elf os.iso build
