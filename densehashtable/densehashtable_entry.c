#include "densehashtable.h"

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

DenseHashTableEntry *dense_hash_table_entry_init(const char *key, const int value) {
    if (key == NULL) {
        return NULL;
    }

    DenseHashTableEntry *entry_ptr =
            (DenseHashTableEntry *) malloc(sizeof(DenseHashTableEntry));
    if (entry_ptr == NULL) {
        return NULL;
    }

    int *hash = calculate_hash(key);
    if (hash == NULL) {
        free(entry_ptr);
        return NULL;
    }

    entry_ptr->key = (char *) malloc(strlen(key) + 1);
    if (entry_ptr->key == NULL) {
        free(hash);
        free(entry_ptr);
        return NULL;
    }

    strcpy(entry_ptr->key, key);
    entry_ptr->hash = *hash;
    entry_ptr->value = value;

    return entry_ptr;
}

int dense_hash_table_entry_destroy(DenseHashTableEntry *entry) {
    if (entry == NULL) {
        return NULLPTR_ERROR;
    }

    if (entry->key != NULL) {
        free(entry->key);
        entry->key = NULL;
    }

    free(entry);
    entry = NULL;

    return ALL_OK;
}

int dense_hash_table_entry_set(DenseHashTableEntry *entry, const char *key, int value) {
    if (entry == NULL || key == NULL) {
        return NULLPTR_ERROR;
    }

    int *hash = calculate_hash(key);
    if (hash == NULL) {
        return NULLPTR_ERROR;
    }

    entry->key = (char *) malloc(strlen(key) + 1);
    if (entry->key == NULL) {
        return NULLPTR_ERROR;
    }
    strcpy(entry->key, key);

    entry->hash = *hash;
    entry->value = value;

    return ALL_OK;
}

int dense_hash_table_entry_print(const DenseHashTableEntry *entry) {
    if (entry == NULL) {
        return NULLPTR_ERROR;
    }

    println("{key: %s; value: %i; hash: %i}",
            entry->key,
            entry->value,
            entry->hash);

    return ALL_OK;
}
