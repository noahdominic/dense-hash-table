#include <stdio.h>
#include <stdlib.h>

#include "densehashtable.h"
#include "rustyc.h"

#define NUM_ITEMS 2

unsigned long long collision_count;

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


    Result res;
    ResultOption resopt;

    for (int i = 0; i < 32; i++) {
        struct DenseHashTable *dht = NULL;
        if ((dht = dense_hash_table_init()) == NULL) {
            continue;
        }

        for (int j = 0; j < i; j++) {
            if (!(res = dense_hash_table_insert(dht, names[j], values[j])).is_ok) {
                printf("Error %i: %s", res.error_code, res.error_message);
            }
        }

        for (int j = 0; j < i; j++) {
            if (!(resopt = dense_hash_table_delete(dht, names[j])).is_ok) {
                printf("Error %i: %s", resopt.error_code, resopt.error_message);
            }
        }

        printf("%u, %lld\n", i, collision_count);
        dense_hash_table_destroy(dht);
    }

    //    for (int i = 0; i < NUM_ITEMS; i++) {
    //        Result res;
    //
    //        res = dense_hash_table_insert(dht, names[i], values[i]);
    //
    //        if (!res.is_ok) {
    //            printf("Error %i: %s", res.error_code, res.error_message);
    //            goto erase_all;
    //        }
    //
    ////        res = dense_hash_table_print(dht);
    ////        if (!res.is_ok) printf("Error %i: %s", res.error_code, res.error_message);
    //    }
    //
    //    {
    //        Result res;
    //
    //        res = dense_hash_table_insert(dht, names[4], values[4]);
    //
    //        if (!res.is_ok) {
    //            printf("Error %i: %s\n", res.error_code, res.error_message);
    //            if (res.error_code != ENTRY_ALREADY_EXISTS) {
    //                goto erase_all;
    //            }
    //        }
    //
    ////        res = dense_hash_table_print(dht);
    ////
    ////        if (!res.is_ok) printf("Error %i: %s\n", res.error_code, res.error_message);
    //    }
    //
    //    for (int i = 0; i < NUM_ITEMS; i++) {
    //        const char *name = names[i];
    //        ResultOption res2 = dense_hash_table_lookup(dht, name);
    //
    //        if (!res2.is_ok) {
    //            printf("Error %i: %s\n", res2.error_code, res2.error_message);
    //        }
    ////        else if (!res2.value.is_some) {
    ////            printf("\"%s\" is not in the hash table.\n", name);
    ////        } else {
    ////            printf("\"%s\" is: %i\n", name, res2.value.value);
    ////        }
    //    }
    //
    //    {
    //        const char *name = NULL;
    //        ResultOption res2 = dense_hash_table_lookup(dht, name);
    //
    //        if (!res2.is_ok) {
    //            printf("Error %i: %s\n", res2.error_code, res2.error_message);
    //        }
    ////        else if (!res2.value.is_some) {
    ////            printf("\"%s\" is not in the hash table.\n", name);
    ////        } else {
    ////            printf("\"%s\" is: %i\n", name, res2.value.value);
    ////        }
    //    }
    //
    //    for (int i = 0; i < NUM_ITEMS; i++) {
    //        const char *name = names[i];
    //        ResultOption res = dense_hash_table_delete(dht, names[i]);
    //
    //        if (!res.is_ok) {
    //            printf("Error %i: %s", res.error_code, res.error_message);
    //            goto erase_all;
    //        }
    //
    //        if (!res.value.is_some) {
    //            printf("\"%s\" is not in the hashtable\n", names[i]);
    //        }

    //        Result res2 = dense_hash_table_print(dht);
    //
    //        if (!res2.is_ok) printf("Error %i: %s", res2.error_code, res2.error_message);
    //    }

erase_all:

    return 0;
}