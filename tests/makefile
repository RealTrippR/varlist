DEBUG_FLAGS := -g -O0

GEN_FLAGS_C :=  -march=core2 -Werror=int-conversion -Werror=pointer-arith  -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type
INCLUDES := -I../llvl_var

LLVL_BUILD_DIR := ../build
TEST_BUILD_DIR := ./build


TEST_BIN := $(TEST_BUILD_DIR)/test.exe

LLVL_BIN := $(LLVL_BUILD_DIR)/llvl_var.o
LLVL_ASM_BIN := $(LLVL_BUILD_DIR)/llvl_var.asm.o


$(TEST_BIN): test.c	test_common.h $(LLVL_BIN) $(LLVL_ASM_BIN)
	gcc $(DEBUG_FLAGS)  -std=c99 $(GEN_FLAGS_C) $(INCLUDES) $(LLVL_ASM_BIN) $(LLVL_BIN) test.c -o $(TEST_BIN)

run: $(TEST_BIN)
	$(TEST_BIN)


TEST_BIN_CC = $(TEST_BUILD_DIR)/test_cc.exe

INCLUDES_CC = -I../boilerplates/cc

$(TEST_BIN_CC): test.cc test_common.hh $(LLVL_BIN) $(LLVL_ASM_BIN)
	g++ $(DEBUG_FLAGS) $(INCLUDES_CC) $(LLVL_ASM_BIN) $(LLVL_BIN) test.cc -o $(TEST_BIN_CC)

cc: $(TEST_BIN_CC)
