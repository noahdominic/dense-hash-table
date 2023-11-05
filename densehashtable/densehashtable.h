#ifndef DENSEHASHTABLE_H
#define DENSEHASHTABLE_H

#define ALL_OK 0
#define NULLPTR_ERROR (-1)
#define ALLOC_ERROR (-2)

#define DHT_INIT_CAPACITY 4
#define DHT_DEFAULT_GROWTH_CONST 2

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

struct DenseHashTableEntry *dense_hash_table_entry_init(const char *key, int value);
int dense_hash_table_entry_destroy(struct DenseHashTableEntry *entry);
int dense_hash_table_entry_set(struct DenseHashTableEntry *entry, const char *key, int value);
int dense_hash_table_entry_print(const struct DenseHashTableEntry *entry);

struct DenseHashTable *dense_hash_table_init();
int dense_hash_table_destroy(struct DenseHashTable *dht);
int dense_hash_table_print(const struct DenseHashTable *dht);
int *dense_hash_table_lookup(struct DenseHashTable *dht, const char *key);
int dense_hash_table_insert(struct DenseHashTable *dht, const char *key, int value);
int *dense_hash_table_remove(struct DenseHashTable *dht, const char *key);

#endif