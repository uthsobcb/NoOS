
CC = gcc                    
AS = nasm                   
LD = ld                     

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -O2

LDFLAGS = -T link.ld -melf_i386

ASFLAGS = -felf32

OBJECTS = loader.o kernel.o memory.o

all: noos.iso
	@echo "NoOS build complete! Use 'make run' to test."

kernel.elf: $(OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) $(OBJECTS) -o kernel.elf

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

loader.o: loader.s
	@echo "Assembling loader..."
	$(AS) $(ASFLAGS) loader.s -o loader.o

noos.iso: kernel.elf
	@echo "Creating ISO image..."
	cp kernel.elf iso/boot/
	grub-mkrescue -o noos.iso iso/

clean:
	@echo "Cleaning build files..."
	rm -f *.o kernel.elf noos.iso *.log
	rm -f iso/boot/kernel.elf

run: noos.iso
	@echo "Starting NoOS in QEMU..."
	qemu-system-i386 -cdrom noos.iso

debug: kernel.elf
	@echo "Starting NoOS with GDB debugging..."
	qemu-system-i386 -cdrom noos.iso -s -S &
	gdb kernel.elf -ex "target remote :1234"

help:
	@echo "NoOS Build System"
	@echo "=================="
	@echo "Available targets:"
	@echo "  all     - Build bootable ISO (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  run     - Run OS in QEMU emulator"
	@echo "  debug   - Run OS with GDB debugging"
	@echo "  help    - Show this help message"

.SECONDARY: $(OBJECTS)

.PHONY: all clean run debug help
