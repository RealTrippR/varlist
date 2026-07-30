cmake_minimum_required(VERSION 3.12)



project(llvl_c_test LANGUAGES C)
add_executable(llvl_c_test test.c test_common.h)

project(llvl_cc_test LANGUAGES CXX)


include_directories(llvl_cc_test ../boilerplates/cc)
add_executable(llvl_cc_test test.cc test_common.hh)


target_link_libraries(llvl_cc_test
    PRIVATE
        llvl_var
)

