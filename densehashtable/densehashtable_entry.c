#include "densehashtable.h"
#include "noahdmncio.h"

#define LEFT_SHIFT_BITS 5

static int *calculate_hash(const char *key) {
    if (key == NULL) {
        return NULL;
    }

    int *hash = (int *) calloc(1, sizeof(int));

    unsigned int len = strlen(key);

    int i;
    for (i = 0; i < len; i++) {
        *hash = (*hash << LEFT_SHIFT_BITS) - *hash + key[i];
    }

    return hash;
}

DenseHashTableEntry *dht_entry_init(const char *key, const int value) {
    if (key == NULL) {
        return NULL;
    }

    DenseHashTableEntry *entry_ptr =
            (DenseHashTableEntry *) malloc(sizeof(DenseHashTableEntry));
    if (entry_ptr == NULL) {
        return NULL;
    }

    int *hash_ptr = calculate_hash(key);
    if (hash_ptr == NULL) {
        free(entry_ptr);
        return NULL;
    }

    entry_ptr->key = (char *) malloc(strlen(key) + 1);
    if (entry_ptr->key == NULL) {
        free(hash_ptr);
        free(entry_ptr);
        return NULL;
    }

    strcpy(entry_ptr->key, key);
    entry_ptr->hash = *hash_ptr;
    entry_ptr->value = value;

    return entry_ptr;
}

int dht_entry_destroy(DenseHashTableEntry *entry_ptr) {
    if (entry_ptr == NULL) {
        return NULLPTR_ERROR;
    }

    if (entry_ptr->key != NULL) {
        free(entry_ptr->key);
        entry_ptr->key = NULL;
    }

    free(entry_ptr);
    entry_ptr = NULL;

    return ALL_OK;
}

int dht_entry_print(DenseHashTableEntry *entry_ptr) {
    if (entry_ptr == NULL) {
        return NULLPTR_ERROR;
    }

    println("{key: %s; value: %i; hash: %i}",
            entry_ptr->key,
            entry_ptr->value,
            entry_ptr->hash
            );

    return ALL_OK;
}
