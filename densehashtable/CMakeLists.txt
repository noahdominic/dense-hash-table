cmake_minimum_required(VERSION 3.10)
project(densehashtable C)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_NAME densehashtable)

set(SRC_FILES
        src/densehashtable_hashfunc.c
        src/densehashtable_entry.c
        src/densehashtable_table.c)

set(INCLUDE_FILES include/densehashtable.h)

add_library(${LIBRARY_NAME} ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NAME} rustyc)
