/*
 * 512-byte x86 BIOS boot sector.
 * Loads a flat 32-bit kernel from disk sector 2 to physical 0x10000,
 * switches to protected mode, and jumps to the C kernel entry.
 */

.code16
.global _start
_start:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov $0x7C00, %sp
    sti
    mov %dl, boot_drive

    mov $boot_msg, %si
    call print_string

    /* Load 32 sectors from CHS 0/0/2 into 0x1000:0x0000 = 0x10000. */
    mov $0x1000, %ax
    mov %ax, %es
    xor %bx, %bx
    mov $0x02, %ah
    mov $32, %al
    mov $0x00, %ch
    mov $0x02, %cl
    mov $0x00, %dh
    mov boot_drive, %dl
    int $0x13
    jc disk_error

    call enable_a20

    cli
    lgdt gdt_descriptor

    mov %cr0, %eax
    or $0x1, %eax
    mov %eax, %cr0
    ljmp $0x08, $protected_mode

print_string:
    lodsb
    test %al, %al
    jz 1f
    mov $0x0E, %ah
    mov $0x00, %bh
    mov $0x07, %bl
    int $0x10
    jmp print_string
1:
    ret

disk_error:
    mov $disk_msg, %si
    call print_string
    hlt
    jmp disk_error

enable_a20:
    in $0x92, %al
    or $0x02, %al
    out %al, $0x92
    ret

.align 8
gdt_start:
    .quad 0x0000000000000000
    .quad 0x00CF9A000000FFFF   /* 0x08: code segment, base 0, limit 4GB */
    .quad 0x00CF92000000FFFF   /* 0x10: data segment, base 0, limit 4GB */
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1
    .long gdt_start

boot_msg:
    .asciz "TinyOS bootloader: loading kernel...\r\n"
disk_msg:
    .asciz "Disk read error.\r\n"
boot_drive:
    .byte 0

.code32
protected_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov $0x90000, %esp
    mov $0x10000, %eax
    call *%eax
halt_loop:
    hlt
    jmp halt_loop

.org 510
.word 0xAA55
