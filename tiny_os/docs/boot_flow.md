# TinyOS Boot Flow

This module builds a bootable x86 BIOS image using GNU binutils.

## Flow

```text
BIOS
  -> loads 512-byte boot sector at 0x7C00
  -> bootloader reads kernel sectors from disk to 0x10000
  -> bootloader enables A20
  -> bootloader installs a GDT
  -> CPU switches from real mode to 32-bit protected mode
  -> bootloader jumps to the C kernel
  -> kernel writes text to VGA memory at 0xB8000
```

## Why it is modernly useful

Even though BIOS boot is older than UEFI, this project is still useful because it exposes the core ideas behind OS bootstrapping: boot sectors, memory layout, protected mode, linker scripts, freestanding C, and raw disk images.

## Build tools

- GNU `as`
- GNU `ld`
- GNU `objcopy`
- `gcc` with 32-bit code generation support
- optional: `qemu-system-i386` for running the image

## Next upgrades

- Interrupt Descriptor Table
- keyboard input
- PIT timer
- memory map detection
- paging
- simple heap allocator
- minimal shell
- UEFI boot path
