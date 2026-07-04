.code32
.global _start
.extern kernel_main
_start:
    call kernel_main
1:
    hlt
    jmp 1b
