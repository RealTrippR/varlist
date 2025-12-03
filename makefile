DEBUG_FLAGS := -g

GEN_FLAGS := -O0 -std=c99 -march=core2 -Werror=int-conversion -Werror=pointer-arith  -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type

BUILD_DIR := ./build
LLVL_BIN := $(BUILD_DIR)/llvl_var.o

ASM_BIN := $(BUILD_DIR)/llvl_var.asm.o
LLVL_DIR := llvl_var

$(ASM_BIN): $(LLVL_DIR)/src/llvl_var.n.asm
	nasm $(DEBUG_FLAGS) -F dwarf -f elf64 $(LLVL_DIR)/src/llvl_var.n.asm -o $(ASM_BIN)


$(LLVL_BIN): $(LLVL_DIR)/llvl_var.89.h  $(LLVL_DIR)/pow_lookup.89.h $(LLVL_DIR)/src/llvl_var.89.c
	gcc -c $(DEBUG_FLAGS) $(GEN_FLAGS) $(LLVL_DIR)/src/llvl_var.89.c -o $(LLVL_BIN)

build: $(LLVL_BIN) $(ASM_BIN)