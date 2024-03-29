/*
 * Copyright 2023, 2024  Noah Dominic Miranda Silvio
 *
 * Licensed under the European Union Public Licence v1.2
 */

#pragma once

#define FREE(ptr)                                                              \
  do {                                                                         \
    free(ptr);                                                                 \
    ptr = 0;                                                                   \
  } while (0)

#define DHT_INIT_CAPACITY 8
#define DHT_DEFAULT_GROWTH_RATE 2

#include "rustyc.h"

enum DhtErrorCode
{
  E_DHT_NULL,        // A DenseHashTable or its components is NULL
  E_DHT_E_NULL,      // A DenseHashTableEntry is NULL
  E_FUNC_PARAM_NULL, // A param called `key` is NULL
  E_ALLOC_FAIL,      // Result of `malloc`/`calloc`/`realloc` is NULL.
};

static const char* DhtErrorMessages[] = {
  "`dht`, `dht->indices`, or `dht->entries` is NULL.\n",
  "A DenseHashTableEntry is NULL.\n",
  "A non-DHT function parameter is NULL.\n",
  "Result of `malloc`/`calloc`/`realloc` is NULL.\n"
};

struct DenseHashTableEntry
{
  char* key;
  int value;
  int hash;
};

struct DenseHashTable
{
  unsigned int size;
  unsigned int capacity;
  int* indices;
  struct DenseHashTableEntry* entries;
};

Result calculate_hash(const char* key);

void dense_hash_table_entry_cleanup(struct DenseHashTableEntry* entry);
Result dense_hash_table_entry_set(struct DenseHashTableEntry* entry,
                                  const char* key,
                                  int value);
Result dense_hash_table_entry_print(const struct DenseHashTableEntry* entry);

struct DenseHashTable* dense_hash_table_init();
Result dense_hash_table_destroy(struct DenseHashTable* dht);
Result dense_hash_table_print(const struct DenseHashTable* dht);
Result dense_hash_table_insert(struct DenseHashTable* dht,
                               const char* key,
                               int value);
ResultOption dense_hash_table_lookup(const struct DenseHashTable* dht,
                                     const char* key);
ResultOption dense_hash_table_remove(struct DenseHashTable* dht,
                                     const char* key);