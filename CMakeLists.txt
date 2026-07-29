cmake_minimum_required(VERSION 3.20)

project(llvl_var LANGUAGES C)

add_subdirectory(llvl/src)
add_subdirectory(llvl)

set(SOURCE_FILES
    llvl_var.89.c
    llvl_var.89.h
)

add_library(llvl_var STATIC ${SOURCE_FILES})

target_include_directories(llvl_var
    PUBLIC
        llvl
)

target_compile_definitions(llvl_var
    PRIVATE
        LLVL_EXEMPT_ASM
)