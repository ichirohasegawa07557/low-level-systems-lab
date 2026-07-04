/*
 * dot_f64_asm(a, b, n) -> double
 * Portable assembly entry points for x86_64 System V and AArch64 ELF/Mach-O-like assemblers.
 * Build systems define HAVE_ASM_DOT=1 when this file is compiled.
 */

#if defined(__x86_64__)
    .text
    .globl dot_f64_asm
    .type dot_f64_asm, @function
dot_f64_asm:
    xorpd %xmm0, %xmm0          /* accumulator */
    test %rdx, %rdx
    je .Ldone_x86
.Lloop_x86:
    movsd (%rdi), %xmm1
    mulsd (%rsi), %xmm1
    addsd %xmm1, %xmm0
    add $8, %rdi
    add $8, %rsi
    dec %rdx
    jne .Lloop_x86
.Ldone_x86:
    ret
    .size dot_f64_asm, .-dot_f64_asm

#elif defined(__aarch64__)
    .text
    .globl dot_f64_asm
dot_f64_asm:
    fmov d0, xzr
    cbz x2, 2f
1:
    ldr d1, [x0], #8
    ldr d2, [x1], #8
    fmadd d0, d1, d2, d0
    subs x2, x2, #1
    b.ne 1b
2:
    ret
#else
    /* Unsupported architecture: the C fallback is used instead. */
#endif
