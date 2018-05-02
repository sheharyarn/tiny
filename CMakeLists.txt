cmake_minimum_required(VERSION 3.8)
project(tiny)

set(CMAKE_C_STANDARD 90s)

set(SOURCE_FILES main.c scanner.c scanner.h globals.h utils.c utils.h errors.h errors.c parser.c parser.h symtab.h symtab.c analyzer.h analyzer.c)
add_executable(tiny ${SOURCE_FILES})

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tiny-codes)
#file(COPY tiny-codes DESTINATION ${CMAKE_BINARY_DIR})

add_custom_command(
        TARGET tiny POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tiny-codes/*
        ${CMAKE_CURRENT_BINARY_DIR}/tiny-codes/
)
