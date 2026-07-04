# ASM Matrix Kernel Design

This module provides numerical kernels implemented in C plus an optional assembly dot-product function.

## Research Motivation

Modern ML, physics simulation, robotics, and quantum simulation systems eventually reduce many operations to vector and matrix kernels. This project exposes the lowest layer behind libraries such as BLAS, NumPy, Eigen, and ML runtime kernels.

## Implemented Kernels

- `dot_f64_c`: portable C dot product
- `dot_f64_asm`: assembly dot product for x86_64 and AArch64 when supported
- `vec_add_f64_c`: vector addition
- `axpy_f64_c`: `y = alpha * x + y`
- `scale_f64_c`: in-place vector scaling
- `l2_norm_f64_c`: Euclidean norm
- `matvec_f64_c`: matrix-vector multiplication
- `matmul_f64_c`: matrix-matrix multiplication

## Portability Strategy

The assembly kernel is optional. If the target architecture is unsupported, the library falls back to the C implementation. This keeps the project usable on Linux, WSL, macOS, and ARM64 environments.

## Benchmarking Notes

The benchmark is intended for comparative experiments, not absolute performance claims. For reliable measurements, run multiple trials, pin CPU frequency when possible, and compare against optimized BLAS libraries as an external baseline.

## Next Upgrades

- AVX2 and AVX-512 implementation
- NEON vectorized AArch64 implementation
- blocked matrix multiplication
- cache-aware benchmarking
- RBM and quantum simulator integration
- microbenchmark CSV export
