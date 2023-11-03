#ifndef DENSEHASHTABLE_H
#define DENSEHASHTABLE_H

#define ALL_OK 0
#define NULLPTR_ERROR -1

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct DenseHashTableEntry {
  char* key;
  int value;
  int hash;
} DenseHashTableEntry;

DenseHashTableEntry* dht_entry_init(const char* key, int value);
int dht_entry_destroy(DenseHashTableEntry* dht_entry_ptr);
int dht_entry_print(DenseHashTableEntry* dht_entry_ptr);

#endif