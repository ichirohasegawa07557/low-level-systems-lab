# Build Verification

This version is designed to run on macOS and Linux using only a standard C compiler and make.

## Verified Commands

```bash
make validate
```

## Expected Final Output

```text
all portable tests passed
full portable validation passed
```

## Validation Scope

The default validation includes:

- Mini CPU assembler and emulator tests
- Matrix kernel unit tests
- Tiny OS source tree validation

The tiny OS boot image build is available as an optional target:

```bash
make os-image
```

That optional target requires an x86-compatible assembler, linker, objcopy, and a freestanding 32-bit C build path.
