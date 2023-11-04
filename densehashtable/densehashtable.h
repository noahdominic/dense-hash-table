#ifndef DENSEHASHTABLE_H
#define DENSEHASHTABLE_H

#define ALL_OK 0
#define NULLPTR_ERROR (-1)
#define ALLOC_ERROR (-2)

#define DHT_INIT_CAPACITY 4

#include "noahdmncio.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct DenseHashTableEntry
{
    char *key;
    int value;
    int hash;
} DenseHashTableEntry;

typedef struct DenseHashTable
{
    unsigned int size;
    unsigned int capacity;
    unsigned int **indices;
    DenseHashTableEntry *entries;
} DenseHashTable;

DenseHashTableEntry *dense_hash_table_entry_init(const char *key, int value);
int dense_hash_table_entry_destroy(DenseHashTableEntry *entry);
int dense_hash_table_entry_set(DenseHashTableEntry *entry, const char* key, int value);
int dense_hash_table_entry_print(const DenseHashTableEntry *entry);

DenseHashTable *dense_hash_table_init();
int dense_hash_table_destroy(DenseHashTable *dht);
int dense_hash_table_print(const DenseHashTable *dht);
int *dense_hash_table_lookup(DenseHashTable *dht, const char *key);
int dense_hash_table_insert(DenseHashTable *dht, int value, const char *key);
int *dense_hash_table_remove(DenseHashTable *dht, const char *key);

#endif