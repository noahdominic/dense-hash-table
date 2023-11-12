#include <stdio.h>
#include <stdlib.h>

#include "densehashtable.h"
#include "rustyc.h"

int main()
{
    const char *names[] = {
            "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace", "Henry",
            "Ivy", "Jack", "Kate", "Liam", "Mia", "Noah", "Olivia", "Peter",
            "Quinn", "Riley", "Sam", "Tyler", "Uma", "Violet", "Will", "Xander",
            "Yara", "Zane", "Ava", "Ben", "Chloe", "Daniel", "Emily", "Felicia"};

    int values[] = {
            42, 57, 33, 78, 23, 56, 89, 64,
            38, 51, 77, 45, 60, 30, 94, 68,
            72, 55, 41, 79, 62, 87, 50, 63,
            96, 75, 80, 53, 36, 67, 98, 69};

    struct DenseHashTable *dht;

    dht = dense_hash_table_init();

    for (int i = 0; i < 10; i++) {
        Result res;

        res = dense_hash_table_insert(dht, names[i], values[i]);

        if (!res.is_ok) {
            printf("Error %i: %s", res.error_code, res.error_message);
            goto erase_all;
        }

        res = dense_hash_table_print(dht);

        if (!res.is_ok) printf("Error %i: %s", res.error_code, res.error_message);
    }

    {
        Result res;

        res = dense_hash_table_insert(dht, names[4], values[4]);

        if (!res.is_ok) {
            printf("Error %i: %s\n", res.error_code, res.error_message);
            if (res.error_code != ENTRY_ALREADY_EXISTS) {
                goto erase_all;
            }
        }

        res = dense_hash_table_print(dht);

        if (!res.is_ok) printf("Error %i: %s\n", res.error_code, res.error_message);
    }

    for (int i = 0; i < 16; i++) {
        const char *name = names[i];
        ResultOption res2 = dense_hash_table_lookup(dht, name);

        if (!res2.is_ok) {
            printf("Error %i: %s\n", res2.error_code, res2.error_message);
        } else if (!res2.value.is_some) {
            printf("\"%s\" is not in the hash table.\n", name);
        } else {
            printf("\"%s\" is: %i\n", name, res2.value.value);
        }
    }

    const char *name = NULL;
    ResultOption res2 = dense_hash_table_lookup(dht, name);

    if (!res2.is_ok) {
        printf("Error %i: %s\n", res2.error_code, res2.error_message);
    } else if (!res2.value.is_some) {
        printf("\"%s\" is not in the hash table.\n", name);
    } else {
        printf("\"%s\" is: %i\n", name, res2.value.value);
    }

erase_all:
    dense_hash_table_destroy(dht);

    printf("Hello, %s", names[0]);

    return 0;
}