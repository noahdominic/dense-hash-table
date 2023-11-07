#include "densehashtable.h"

struct DenseHashTableEntry *dense_hash_table_entry_init(const char *key, const int value)
{
    if (key == NULL) {
        return NULL;
    }

    int *hash;
    hash = calculate_hash(key);

    if (hash == NULL) {
        return NULL;
    }

    struct DenseHashTableEntry *entry_ptr =
            (struct DenseHashTableEntry *) malloc(sizeof(struct DenseHashTableEntry));
    if (entry_ptr == NULL) {
        return NULL;
    }

    entry_ptr->key = (char *) malloc(strlen(key) + 1);
    if (entry_ptr->key == NULL) {
        free(entry_ptr);
        return NULL;
    }

    strcpy(entry_ptr->key, key);
    entry_ptr->value = value;
    entry_ptr->hash = *hash;

    return entry_ptr;
}

int dense_hash_table_entry_destroy(struct DenseHashTableEntry *entry)
{
    if (entry == NULL) {
        return NULLPTR_ERROR;
    }

    if (entry->key != NULL) {
        free(entry->key);
        entry->key = NULL;
    }

    entry->value = 0;
    entry->hash = 0;
    free(entry);
    entry = NULL;

    return ALL_OK;
}

int dense_hash_table_entry_set(struct DenseHashTableEntry *entry, const char *key, const int value)
{
    if (entry == NULL || key == NULL) {
        return NULLPTR_ERROR;
    }

    int *hash;
    hash = calculate_hash(key);
    if (hash == NULL) {
        return NULLPTR_ERROR;
    }

    entry->key = (char *) malloc(strlen(key) + 1);
    if (entry->key == NULL) {
        return NULLPTR_ERROR;
    }

    strcpy(entry->key, key);
    entry->value = value;
    entry->hash = *hash;

    return ALL_OK;
}

int dense_hash_table_entry_print(const struct DenseHashTableEntry *entry)
{
    if (entry == NULL) {
        return NULLPTR_ERROR;
    }

    printf("{key: %s;\tvalue: %i;\thash: %i}\n",
           entry->key,
           entry->value,
           entry->hash);

    return ALL_OK;
}
