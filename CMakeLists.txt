cmake_minimum_required(VERSION 3.20)

project(llvl_var LANGUAGES C)



set(SOURCE_FILES
    llvl_var/src/llvl_var.89.c
    llvl_var/llvl_var.89.h
)

add_library(llvl_var STATIC ${SOURCE_FILES})

target_include_directories(llvl_var
    PUBLIC
        llvl_var
)

target_compile_definitions(llvl_var
    PRIVATE
        LLVL_EXEMPT_ASM
)


add_library(llvl_var_cc INTERFACE)

target_include_directories(llvl_var_cc
    INTERFACE
        boilerplates/cc
)

target_link_libraries(llvl_var_cc
    INTERFACE
        llvl_var
)



add_subdirectory(tests)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    if(WIN32)
        target_sources(llvl_var PRIVATE
            llvl_var/deps/streql/streql_x64_win.obj
            llvl_var/deps/streql/strneql_x64_win.obj
        )
    else()
        target_sources(llvl_var PRIVATE
            llvl_var/deps/streql/streql_x64_unix.o
            llvl_var/deps/streql/strneql_x64_unix.o
        )
    endif()
endif()