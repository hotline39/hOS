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


$(BUILD)/timer.o: kernel/timer.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/keyboard.o: kernel/keyboard.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD)/shell.o: kernel/shell.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/pmm.o: kernel/pmm.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/paging.o: kernel/paging.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/heap.o: kernel/heap.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/process.o: kernel/process.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/context_switch.o: kernel/context_switch.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/scheduler.o: kernel/scheduler.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/ramfs.o: kernel/ramfs.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/fat12.o: kernel/fat12.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

fat12.img: tools/mkfat12.py
	python3 tools/mkfat12.py

$(BUILD)/fat12_img.o: fat12.img | $(BUILD)
	objcopy -I binary -O elf32-i386 -B i386 $< $@

$(BUILD)/syscall.o: kernel/syscall.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/user.o: kernel/user.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/user_asm.o: kernel/user.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/elf.o: kernel/elf.c | $(BUILD)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(BUILD)/process_start.o: kernel/process_start.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(KERNEL): \
	$(BUILD)/boot.o \
	$(BUILD)/kernel.o \
	$(BUILD)/vga.o \
	$(BUILD)/gdt.o \
	$(BUILD)/gdt_flush.o \
	$(BUILD)/idt.o \
	$(BUILD)/interrupt.o \
	$(BUILD)/pic.o \
	$(BUILD)/timer.o \
	$(BUILD)/keyboard.o \
	$(BUILD)/shell.o \
	$(BUILD)/pmm.o \
	$(BUILD)/paging.o \
	$(BUILD)/heap.o \
	$(BUILD)/process.o \
	$(BUILD)/context_switch.o \
	$(BUILD)/scheduler.o \
	$(BUILD)/ramfs.o \
	$(BUILD)/fat12.o \
	$(BUILD)/fat12_img.o \
	$(BUILD)/syscall.o \
	$(BUILD)/user.o \
	$(BUILD)/user_asm.o \
	$(BUILD)/elf.o \
	$(BUILD)/process_start.o \
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