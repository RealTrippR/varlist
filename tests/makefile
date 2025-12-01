DEBUG_FLAGS := -g

GEN_FLAGS := -O0 -std=c99 -march=core2 -Werror=int-conversion -Werror=pointer-arith  -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type
INCLUDES := -I../llvl_env

LLVL_BUILD_DIR := ../build
TEST_BUILD_DIR := ./build


TEST_BIN := $(TEST_BUILD_DIR)/test.exe

LLVL_BIN := $(LLVL_BUILD_DIR)/llvl_env.o
LLVL_ASM_BIN := $(LLVL_BUILD_DIR)/llvl_env.asm.o


$(TEST_BIN): test.c	test_common.h $(LLVL_BIN) $(LLVL_ASM_BIN)
	gcc $(DEBUG_FLAGS) $(GEN_FLAGS) $(INCLUDES) $(LLVL_ASM_BIN) $(LLVL_BIN) test.c -o $(TEST_BIN)

run: $(TEST_BIN)
	$(TEST_BIN)