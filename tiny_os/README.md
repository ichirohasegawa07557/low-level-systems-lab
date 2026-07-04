# tiny-os-bootloader-asm

A bootable educational x86 tiny OS project.

It includes:

- 512-byte BIOS boot sector in assembly
- disk-sector kernel loader
- A20 enabling
- GDT setup
- switch to 32-bit protected mode
- freestanding C kernel
- VGA text output
- raw boot image generation

## Build Image

```bash
make image
```

The output is:

```text
dist/tinyos.img
```

## Run with QEMU

```bash
make run
```

If QEMU is not installed, install `qemu-system-i386` first.

## Expected Screen

```text
TinyOS C kernel is running.
Mode: 32-bit protected mode
VGA text output: OK
Kernel loaded at: 0x00010000
```

## Practical and Research Value

This is a BIOS boot path, not UEFI. It is intentionally small so the CPU transition and memory layout are easy to understand. It is useful for studying boot sectors, protected mode, linker scripts, raw disk images, freestanding C, and the early stages of operating-system design.

## Next Upgrades

- Interrupt Descriptor Table
- keyboard input
- PIT timer
- memory map detection
- paging
- simple heap allocator
- minimal shell
- UEFI boot path
