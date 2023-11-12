#include "densehashtable.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dense_hash_table_entry_destroy(struct DenseHashTableEntry *entry)
{
    if (entry == NULL) {
        return;
    }

    if (entry->key != NULL) {
        free(entry->key);
        entry->key = NULL;
    }

    entry->value = 0;
    entry->hash = 0;
}

Result dense_hash_table_entry_set(
        struct DenseHashTableEntry *entry,
        const char *key,
        const int value)
{
    if (entry == NULL || key == NULL) {
        return Err(NULPTR_ERR,
                   "dense_hash_table_entry_set() params `entry` incl-or `key` is NULL");
    }

    int hash;
    const Result res = calculate_hash(key);
    if (res.is_ok) {
        hash = res.value;
    } else {
        return res;
    }

    if (entry->key != NULL) {
        free(entry->key);
        entry->key = NULL;
    }

    if ((entry->key = malloc(strlen(key) + 1)) == NULL) {
        return Err(ALLOC_FAIL_ERR,
                   "In `dense_hash_table_entry_set()`, malloc for `entry->key` failed.");
    }

    strcpy(entry->key, key);
    entry->value = value;
    entry->hash = hash;

    return Ok_empty();
}

Result dense_hash_table_entry_print(const struct DenseHashTableEntry *entry)
{
    if (entry == NULL) {
        return Err(NULPTR_ERR, "`dense_hash_table_entry_print()` param `entry` is NULL.");
    }

    printf("{key: %s\tvalue: %i\thash: %i}\n",
           entry->key,
           entry->value,
           entry->hash);

    return Ok_empty();
}