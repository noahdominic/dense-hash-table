cmake_minimum_required(VERSION 3.26)
project(dense_hash_tables C)

set(CMAKE_C_STANDARD 90)

include_directories(noahdmncio
        densehashtable
)

add_executable(dense_hash_tables
        noahdmncio/noahdmncio.c
        noahdmncio/noahdmncio.h
        densehashtable/densehashtable.h
        main.c
        densehashtable/densehashtable_entry.c
        densehashtable/densehashtable_table.c
)
