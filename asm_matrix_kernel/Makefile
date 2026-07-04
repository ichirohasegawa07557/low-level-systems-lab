CC ?= cc
CFLAGS ?= -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O3 -Iinclude
LDFLAGS ?= -lm
UNAME_S := $(shell uname -s)
ARCH := $(shell uname -m)
USE_ASM ?= 0
ASM_OBJ :=
ASM_DEFINE :=
ifeq ($(USE_ASM),1)
ifneq ($(UNAME_S),Darwin)
ifneq (,$(filter $(ARCH),x86_64 aarch64))
ASM_OBJ := build/dot_f64_asm.o
ASM_DEFINE := -DHAVE_ASM_DOT=1
endif
endif
endif
.PHONY: all clean test bench asm-test
all: bin/test_matrix bin/benchmark
build/matrix.o: src/matrix.c include/matrix.h
	@mkdir -p build
	$(CC) $(CFLAGS) $(ASM_DEFINE) -c $< -o $@
build/dot_f64_asm.o: src/dot_f64_asm.S
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@
bin/test_matrix: tests/test_matrix.c build/matrix.o $(ASM_OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
bin/benchmark: examples/benchmark.c build/matrix.o $(ASM_OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
test: bin/test_matrix
	./bin/test_matrix
bench: bin/benchmark
	./bin/benchmark 100000
asm-test:
	$(MAKE) clean
	$(MAKE) USE_ASM=1 test
clean:
	rm -rf build bin
