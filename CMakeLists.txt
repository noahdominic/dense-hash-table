cmake_minimum_required(VERSION 3.6)

project(dense_hash_tables C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(noahdmncio
        densehashtable
)

add_subdirectory(noahdmncio)
add_subdirectory(densehashtable)

add_executable(out main.c)

target_link_libraries(out noahdmncio densehashtable)
