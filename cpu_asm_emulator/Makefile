CC ?= gcc
CFLAGS ?= -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O2 -Iinclude
LDFLAGS ?=
SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
BIN := bin/minicpu

.PHONY: all clean test examples research-check
all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c include/*.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

examples: $(BIN)
	@mkdir -p build/programs
	$(BIN) asm programs/add.asm build/programs/add.bin
	$(BIN) run build/programs/add.bin
	$(BIN) asm programs/loop.asm build/programs/loop.bin
	$(BIN) run build/programs/loop.bin
	$(BIN) asm programs/factorial.asm build/programs/factorial.bin
	$(BIN) run build/programs/factorial.bin
	$(BIN) asm programs/fibonacci.asm build/programs/fibonacci.bin
	$(BIN) run build/programs/fibonacci.bin
	$(BIN) asm programs/memory_sum.asm build/programs/memory_sum.bin
	$(BIN) run --dump-memory 200 5 build/programs/memory_sum.bin

research-check: $(BIN)
	@mkdir -p build/programs
	$(BIN) asm programs/factorial.asm build/programs/factorial.bin
	$(BIN) disasm build/programs/factorial.bin > build/programs/factorial.disasm
	$(BIN) run --trace --max-steps 1000 build/programs/factorial.bin > build/programs/factorial.trace
	@echo "research artifacts written to build/programs"

test: examples research-check
	@echo "cpu_asm_emulator tests passed"

clean:
	rm -rf build bin
