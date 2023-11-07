#pragma once

#define NULPTR_ERR 100

#define DHT_INIT_CAPACITY 8
#define DHT_DEFAULT_GROWTH_RATE 2

#include "rustyc.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct DenseHashTableEntry
{
    char *key;
    int value;
    int hash;
};

struct DenseHashTable
{
    unsigned int size;
    unsigned int capacity;
    unsigned int **indices;
    struct DenseHashTableEntry *entries;
};


Result calculate_hash(const char *key);

void dense_hash_table_entry_destroy(struct DenseHashTableEntry *entry);
Result dense_hash_table_entry_set(struct DenseHashTableEntry *entry, const char *key, int value);
Result dense_hash_table_entry_print(const struct DenseHashTableEntry *entry);


