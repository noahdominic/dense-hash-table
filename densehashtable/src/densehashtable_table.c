#include "densehashtable.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Result s_dense_hash_table_register_entry(
        const struct DenseHashTable *dht,
        const int hash,
        const unsigned int index_of_entry)
{
    srand(clock());

    const unsigned int mask = dht->capacity;

    unsigned int candidate_idx = hash % mask;

    while (dht->indices[candidate_idx] != NULL) {
        candidate_idx = (candidate_idx + 1) % mask;
    }

    if ((dht->indices[candidate_idx] = malloc(sizeof(int))) == NULL) {
        return Err(ALLOC_FAIL_ERR,
                   "From `s_dense_hash_table_register_entry()`: `dht->indices[candidate_idx]` cannot be malloced");
    }
    *dht->indices[candidate_idx] = index_of_entry;

    return Ok_empty();
}

static Result s_dense_hash_table_grow(struct DenseHashTable *dht)
{
    unsigned int **new_indices;
    if ((new_indices = calloc(dht->capacity * DHT_DEFAULT_GROWTH_RATE, sizeof(unsigned int *))) == NULL) {
        return Err(ALLOC_FAIL_ERR, "From s_dense_hash_table_grow: Alloc fail");
    }

    /*
     * Free old indices
     */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }
    free(dht->indices);
    /*
     * Calculate new indices
     */
    dht->indices = new_indices;
    dht->capacity *= DHT_DEFAULT_GROWTH_RATE;
    for (unsigned int i = 0; i < dht->size; i++) {
        const Result res = s_dense_hash_table_register_entry(dht, dht->entries[i].hash, i);
        if (!res.is_ok) {
            return res;
        }
    }

    return Ok_empty();
}

static Result s_dense_hash_table_shrink(struct DenseHashTable *dht)
{
    /*
     * Free old indices
     */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }
    free(dht->indices);
    /*
     * Calculate new indices
     */
    dht->capacity = dht->capacity / DHT_DEFAULT_GROWTH_RATE;
    dht->indices = calloc(dht->capacity, sizeof(unsigned int *));
    for (unsigned int i = 0; i < dht->size; i++) {
        const Result res = s_dense_hash_table_register_entry(dht, dht->entries[i].hash, i);
        if (!res.is_ok) {
            return res;
        }
    }

    return Ok_empty();
}

static Result s_dense_hash_table_refresh_indices(const struct DenseHashTable *dht)
{
    // ? I think this check below is unreachable.
    // if (dht == NULL) {
    //     return Err(NULPTR_ERR, "From `s_dense_hash_table_refresh_indices()`: param `dht` is NULL");
    // }

    /*
     * Free old indices
     */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            dht->indices[i] = NULL;
        }
    }
    /*
     * Calculate new indices
     */
    for (unsigned int i = 0; i < dht->size; i++) {
        const Result res = s_dense_hash_table_register_entry(dht, dht->entries[i].hash, i);
        if (!res.is_ok) {
            return res;
        }
    }
    return Ok_empty();
}

struct DenseHashTable *dense_hash_table_init()
{
    struct DenseHashTable *dht;
    if ((dht = malloc(sizeof(struct DenseHashTable))) == NULL) {
        return NULL;
    }

    if ((dht->indices = calloc(DHT_INIT_CAPACITY, sizeof(unsigned int *))) == NULL) {
        free(dht);
        dht = NULL;
        return NULL;
    }

    dht->capacity = DHT_INIT_CAPACITY;
    dht->size = 0;

    return dht;
}

Result dense_hash_table_destroy(struct DenseHashTable *dht)
{
    if (dht != NULL) {
        if (dht->entries != NULL) {
            for (unsigned int i = 0; i < dht->size; i++) {
                if (dht->entries[i].key != NULL) {
                    free(dht->entries[i].key);
                    dht->entries[i].key = NULL;
                }
            }
            free(dht->entries);
            dht->entries = NULL;
        }

        if (dht->indices != NULL) {
            for (unsigned int i = 0; i < dht->capacity; i++) {
                if (dht->indices[i] != NULL) {
                    free(dht->indices[i]);
                    dht->indices[i] = NULL;
                }
            }
            free(dht->indices);
            dht->indices = NULL;
        }

        free(dht);
        dht = NULL;
    }

    return Ok_empty();
}

Result dense_hash_table_print(const struct DenseHashTable *dht)
{
    if (dht == NULL) {
        return Err(NULPTR_ERR, "From `dense_hash_table_print(): param `dht` is NULL");
    }

    if (dht->indices == NULL) {
        return Err(DHT_NOT_INITIALISED, "From `dense_hash_table_print(): param `dht` is uninitialised");
    }

    printf("Size:%u\tCapacity:%u\n", dht->size, dht->capacity);

    printf("The contents of `indices`: ");
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] == NULL) {
            printf("NUL...");
        } else {
            printf("%u...", *dht->indices[i]);
        }
    }
    printf("\n");

    // The code below will fail if `dht->entries` == NULL.
    // However, we are hoping that nothing goes wrong
    // because when nothing goes wrong `dht->entries` is only NULL
    // when dht->size = 0;
    printf("The contents of `entries`: [\n");
    for (unsigned int i = 0; i < dht->size; i++) {
        dense_hash_table_entry_print(&dht->entries[i]);
    }
    printf("]\n");

    return Ok_empty();
}

Result dense_hash_table_insert(
        struct DenseHashTable *dht,
        const char *key,
        const int value)
{
    /*
        This flow seems to be iffy.  There's too many things that can go wrong.
        Would be preferable to change this such that a duplicate `dht` is made
        so that any errors that can happen will not affect the original.  Only
        when everything goes perfectly will the new table 
        be moved to the original.
    */
    if (dht == NULL || key == NULL) {
        return Err(
                NULPTR_ERR,
                "From `dense_hash_table_insert()`: params `key` or `dht` is/are NULL.");
    }

    if (dht->size + 1 > dht->capacity) {
        const Result res = s_dense_hash_table_grow(dht);
        if (!res.is_ok) {
            return res;
        }
    }

    /* 
     * Appropriate index is found. Now let's add the DHT entry to the list of entries.
     */
    struct DenseHashTableEntry *new_entries;
    if ((new_entries = realloc(dht->entries, (dht->size + 1) * sizeof(struct DenseHashTableEntry))) == NULL) {
        return Err(ALLOC_FAIL_ERR, "From `dense_hash_table_insert()`: `dht->entries` cannot be reallocated.");
    }
    dht->entries = new_entries;

    Result res = dense_hash_table_entry_set(&dht->entries[dht->size], key, value);
    if (!res.is_ok) {
        return res;
    }

    res = s_dense_hash_table_register_entry(dht, dht->entries[dht->size].hash, dht->size);
    if (!res.is_ok) {
        return res;
    }

    dht->size++;

    return Ok_empty();
}

ResultOption dense_hash_table_lookup(const struct DenseHashTable *dht, const char *key)
{
    if (dht == NULL || key == NULL) {
        return Err_option(
                NULPTR_ERR,
                "From `dense_hash_table_lookup()`: params `key` or `dht` is/are NULL.");
    }

    int hash;
    const Result res = calculate_hash(key);
    if (res.is_ok) {
        hash = res.value;
    } else {
        return Err_option(res.error_code, res.error_message);
    }

    const unsigned int mask = dht->capacity;

    unsigned int candidate_idx = hash % mask;
    const unsigned int first_candidate = candidate_idx;

    if (dht->indices[candidate_idx] == NULL) {
        return Ok_option(None());
    } else {
        do {
            if (dht->indices[candidate_idx] != NULL) {
                if (strcmp(dht->entries[*dht->indices[candidate_idx]].key, key) == 0) {
                    return Ok_option(Some(candidate_idx));
                }
            }

            candidate_idx = (candidate_idx + 1) % mask;
        } while (candidate_idx != first_candidate);
    }
    return Ok_option(None());
}

ResultOption dense_hash_table_delete(struct DenseHashTable *dht, const char *key)
{
    /* Input validation */
    if (dht == NULL || key == NULL) {
        return Err_option(
                NULPTR_ERR,
                "From `dense_hash_table_delete()`: params `key` or `dht` is/are NULL.");
    }

    /* Getting the index in indices */
    const ResultOption res = dense_hash_table_lookup(dht, key);
    if (!res.is_ok || !res.value.is_some) {
        return res;
    }
    const int idx_in_indices = res.value.value;

    printf("The contents of `indices`: ");
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] == NULL) {
            printf("NUL...");
        } else {
            printf("%u...", *dht->indices[i]);
        }
    }
    printf("\n");

    printf("The address of `indices`: ");
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] == NULL) {
            printf("NUL...");
        } else {
            printf("%x...", dht->indices[i]);
        }
    }
    printf("\n");

    /* Remove entries[indices[idx]] from the list */
    for (unsigned int i = *dht->indices[idx_in_indices]; i < dht->size - 1; i++) {
        const Result res2 = dense_hash_table_entry_set(&dht->entries[i], dht->entries[i + 1].key, dht->entries[i + 1].value);
        if (!res2.is_ok) {
            return Err_option(res2.error_code, res2.error_message);
        }
    }

    /* Reset indices[idx] to NULL */
    if (dht->indices[idx_in_indices] != NULL) {
        free(dht->indices[idx_in_indices]);
        dht->indices[idx_in_indices] = NULL;
    }

    printf("\n");

    printf("The address of `indices`: ");
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] == NULL) {
            printf("NUL...");
        } else {
            printf("%x...", dht->indices[i]);
        }
    }
    printf("\n");

    dht->size--;

    dense_hash_table_entry_destroy(&(dht->entries[dht->size]));

    struct DenseHashTableEntry *new_entries = NULL;

    if ((new_entries = realloc(dht->entries, dht->size * sizeof(struct DenseHashTableEntry))) == NULL && dht->size > 0) {

        return Err_option(ALLOC_FAIL_ERR,
                          "From `dense_hash_table_delete()`: `dht->entries` realloc failed.");
    }

    dht->entries = new_entries;

    if (dht->size <= dht->capacity / DHT_DEFAULT_GROWTH_RATE & dht->capacity > DHT_INIT_CAPACITY) {
        const Result res2 = s_dense_hash_table_shrink(dht);
        if (res2.is_ok == 0) {
            return Err_option(res2.error_code, res2.error_message);
        }
    } else {
        const Result res2 = s_dense_hash_table_refresh_indices(dht);
        if (res2.is_ok == 0) {
            return Err_option(res2.error_code, res2.error_message);
        }
    }

    return Ok_option(Some(idx_in_indices));

}
