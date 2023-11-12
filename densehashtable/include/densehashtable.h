#pragma once

#define NULPTR_ERR 100
#define ALLOC_FAIL_ERR 101
#define DHT_NOT_INITIALISED 102

#define DHT_INIT_CAPACITY 8
#define DHT_DEFAULT_GROWTH_RATE 2

#include "rustyc.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

struct DenseHashTable *dense_hash_table_init();
Result dense_hash_table_destroy(struct DenseHashTable *dht);
Result dense_hash_table_print(const struct DenseHashTable *dht);
Result dense_hash_table_insert(struct DenseHashTable *dht, const char *key, int value);
ResultOption dense_hash_table_lookup(struct DenseHashTable *dht, const char *key);
ResultOption dense_hash_table_delete(struct DenseHashTable *dht, const char *key);