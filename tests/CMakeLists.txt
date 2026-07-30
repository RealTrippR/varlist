cmake_minimum_required(VERSION 3.12)



project(llvl_c_test LANGUAGES C)
add_executable(llvl_c_test test.c test_common.h)

set_property(TARGET llvl_c_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests")

project(llvl_cc_test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(llvl_cc_test ../boilerplates/cc)
add_executable(llvl_cc_test test.cc test_common.hh)

set_target_properties(llvl_cc_test PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

set_property(TARGET llvl_cc_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests")

target_link_libraries(llvl_cc_test
    PRIVATE
        llvl_var
)

