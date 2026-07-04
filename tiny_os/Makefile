AS ?= as
CC ?= cc
LD ?= ld
OBJCOPY ?= objcopy
QEMU ?= qemu-system-i386
BUILD := build
DIST := dist
IMAGE := $(DIST)/tinyos.img
KERNEL_SECTORS := 32
.PHONY: all check image run clean check-tools
all: check
check:
	@test -f boot/bootloader.S
	@test -f kernel/entry.S
	@test -f kernel/kernel.c
	@test -f kernel/linker.ld
	@echo "tiny_os source check passed"
check-tools:
	@command -v $(AS) >/dev/null 2>&1 || (echo "assembler not found"; exit 1)
	@command -v $(CC) >/dev/null 2>&1 || (echo "C compiler not found"; exit 1)
	@command -v $(LD) >/dev/null 2>&1 || (echo "linker not found"; exit 1)
	@command -v $(OBJCOPY) >/dev/null 2>&1 || (echo "objcopy not found. On macOS, install an x86 ELF toolchain before building the boot image."; exit 1)
$(BUILD)/bootloader.o: boot/bootloader.S
	@mkdir -p $(BUILD)
	$(AS) --32 $< -o $@
$(BUILD)/bootloader.bin: $(BUILD)/bootloader.o
	$(LD) -m elf_i386 -N -Ttext 0x7C00 --oformat binary $< -o $@
	@size=$$(wc -c < $@); test $$size -eq 512 || (echo "bootloader must be 512 bytes, got $$size"; exit 1)
$(BUILD)/entry.o: kernel/entry.S
	@mkdir -p $(BUILD)
	$(AS) --32 $< -o $@
$(BUILD)/kernel.o: kernel/kernel.c
	@mkdir -p $(BUILD)
	$(CC) -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -nostdlib -Wall -Wextra -O2 -c $< -o $@
$(BUILD)/kernel.elf: $(BUILD)/entry.o $(BUILD)/kernel.o kernel/linker.ld
	$(LD) -m elf_i386 -T kernel/linker.ld -nostdlib $(BUILD)/entry.o $(BUILD)/kernel.o -o $@
$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary $< $@
	@size=$$(wc -c < $@); max=$$(( $(KERNEL_SECTORS) * 512 )); test $$size -le $$max || (echo "kernel too large: $$size > $$max"; exit 1)
image: check-tools $(BUILD)/bootloader.bin $(BUILD)/kernel.bin
	@mkdir -p $(DIST)
	dd if=/dev/zero of=$(IMAGE) bs=512 count=$$((1 + $(KERNEL_SECTORS))) status=none
	dd if=$(BUILD)/bootloader.bin of=$(IMAGE) conv=notrunc status=none
	dd if=$(BUILD)/kernel.bin of=$(IMAGE) bs=512 seek=1 conv=notrunc status=none
	@echo "created $(IMAGE)"
run: image
	@if command -v $(QEMU) >/dev/null 2>&1; then \
		$(QEMU) -drive format=raw,file=$(IMAGE) -no-reboot; \
	else \
		echo "qemu-system-i386 not found. Install QEMU, then run: make run"; \
	fi
clean:
	rm -rf $(BUILD) $(DIST)
