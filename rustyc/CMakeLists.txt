cmake_minimum_required(VERSION 3.10)
project(rustyc C)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_NAME rustyc)

set(SRC_FILES src/rustyc.c)

set(INCLUDE_FILES include/rustyc.h)

add_library(${LIBRARY_NAME} ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
