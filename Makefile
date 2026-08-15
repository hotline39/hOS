ASM=nasm
CC=gcc
LD=ld
GRUB=grub-mkrescue


CFLAGS=-m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nodefaultlibs -nostartfiles
LDFLAGS=-m elf_i386 -T linker.ld


BUILD=build
ISO=isodir
KERNEL=$(BUILD)/hos.bin
ISO_FILE=hos.iso


all: $(ISO_FILE)


$(BUILD):
	mkdir -p $(BUILD)


$(BUILD)/boot.o: boot/boot.asm | $(BUILD)
	$(ASM) -f elf32 $< -o $@


$(BUILD)/gdt_flush.o: boot/gdt_flush.asm | $(BUILD)
	$(ASM) -f elf32 $< -o $@


$(BUILD)/idt_load.o: boot/idt_load.asm | $(BUILD)
	$(ASM) -f elf32 $< -o $@


$(BUILD)/isr.o: boot/isr.asm | $(BUILD)
	$(ASM) -f elf32 $< -o $@


$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/vga.o: drivers/vga.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/gdt.o: kernel/gdt.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/idt.o: kernel/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/interrupt.o: kernel/interrupt.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/pic.o: kernel/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(KERNEL): \
	$(BUILD)/boot.o \
	$(BUILD)/kernel.o \
	$(BUILD)/vga.o \
	$(BUILD)/gdt.o \
	$(BUILD)/gdt_flush.o \
	$(BUILD)/idt.o \
	$(BUILD)/interrupt.o \
	$(BUILD)/pic.o \
	$(BUILD)/idt_load.o \
	$(BUILD)/isr.o
	$(LD) $(LDFLAGS) -o $@ $^


$(ISO_FILE): $(KERNEL) grub.cfg
	mkdir -p $(ISO)/boot/grub
	cp $(KERNEL) $(ISO)/boot/hos.bin
	cp grub.cfg $(ISO)/boot/grub/grub.cfg
	$(GRUB) -o $@ $(ISO)


clean:
	rm -rf $(BUILD) $(ISO) $(ISO_FILE)


run: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE)