#include "densehashtable/densehashtable.h"
#include "noahdmncio/noahdmncio.h"

int main() {
    const char *name = "Kyle";
    println("Hello, %s", name);

    error = 0;

    const char* names[] = {
            "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Henry",
            "Ivy", "Jack", "Kate", "Liam", "Mia", "Noah", "Olivia", "Peter",
            "Quinn", "Riley", "Sam", "Tyler", "Uma", "Violet", "Will", "Xander",
            "Yara", "Zane", "Ava", "Ben", "Chloe", "Daniel", "Emily"
    };

    int values[] = {
            42, 57, 33, 78, 23, 56, 89, 64,
            38, 51, 77, 45, 60, 30, 94, 68,
            72, 55, 41, 79, 62, 87, 50, 63,
            96, 75, 80, 53, 36, 67, 98
    };

    for (i = 0; i < 31; i++) {
        println("-------");
        error += dense_hash_table_insert(dht, names[i % 32], values[i % 32]);
        error += dense_hash_table_print(dht);
    }


    error += dense_hash_table_destroy(dht);

    return error;
}
