cmake_minimum_required(VERSION 3.16)
project(renderer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(RUN_CLANG_TIDY run-clang-tidy)

if(RUN_CLANG_TIDY)
    add_custom_target(tidy
        COMMAND ${RUN_CLANG_TIDY} -p ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy"
    )
endif()

add_subdirectory(core)
