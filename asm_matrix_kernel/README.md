# asm-matrix-kernel

A C and assembly numerical kernel project for low-level performance analysis.

It provides portable C matrix operations and an optional assembly-backed `double` dot product.

## Build

```bash
make
```

## Test

```bash
make test
```

## Benchmark

```bash
make bench
```

Example output:

```text
n=1000000
dot_f64_c   = ... time=...
dot_f64_asm = ... time=...
axpy_f64_c  = completed time=...
l2_norm_f64 = ... time=...
match=yes
```

## Implemented Kernels

- `dot_f64_c`
- `dot_f64_asm`
- `vec_add_f64_c`
- `axpy_f64_c`
- `scale_f64_c`
- `l2_norm_f64_c`
- `matvec_f64_c`
- `matmul_f64_c`

## Supported Assembly Targets

- x86_64: scalar SSE2 dot product
- AArch64: scalar floating-point dot product
- other architectures: portable C fallback

## Why This Is Useful

- Connects C, assembly, and numerical computing
- Helps explain how ML and physics kernels are optimized
- Can be reused by RBM, quantum simulation, and robotics projects
- Provides a foundation for AVX2, AVX-512, NEON, and cache-aware kernels
