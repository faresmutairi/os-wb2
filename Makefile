CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-T source/link.ld -ffreestanding -O2 -nostdlib

OBJS = source/loader.o source/kernel.o drivers/framebuffer.o

all: kernel.elf os.iso

source/loader.o: source/loader.asm
	nasm -f elf source/loader.asm -o source/loader.o

source/kernel.o: source/kernel.c
	$(CC) $(CFLAGS) -c source/kernel.c -o source/kernel.o

drivers/framebuffer.o: drivers/framebuffer.c
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

kernel.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o kernel.elf $(OBJS)

os.iso:
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp boot/grub/menu.lst iso/boot/grub/
	cp boot/grub/stage2_eltorito iso/boot/grub/
	genisoimage -R -b boot/grub/stage2_eltorito \
	-no-emul-boot -boot-load-size 4 -A os -input-charset utf8 \
	-quiet -boot-info-table -o os.iso iso

run:
	qemu-system-i386 -curses -boot d -cdrom os.iso -m 32 \
	-monitor telnet::45454,server,nowait -serial mon:stdio

clean:
	rm -rf *.o *.elf os.iso iso
	rm -rf source/*.o drivers/*.o
