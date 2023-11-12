#include "densehashtable.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned int s_update_idx(const unsigned int candidate_idx, const unsigned int mask, const unsigned int num_attempts)
{
    collision_count++;
    return (candidate_idx + 1) % mask;
}

static Result s_dense_hash_table_register_entry(
        const struct DenseHashTable *dht,
        const int hash,
        const unsigned int index_of_entry)
{
    /* Find free slot in table */
    const unsigned int mask = dht->capacity;
    unsigned int candidate_idx = hash % mask;
    unsigned int num_tries = 1;
    while (dht->indices[candidate_idx] != NULL) {
        candidate_idx = s_update_idx(candidate_idx, mask, num_tries);
    }

    /* Add int to the free slot */
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

    /* Free old indices */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }
    free(dht->indices);

    /* Calculate new indices */
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
    unsigned int **new_indices;
    if ((new_indices = calloc(dht->capacity / DHT_DEFAULT_GROWTH_RATE, sizeof(unsigned int *))) == NULL) {
        return Err(ALLOC_FAIL_ERR, "From s_dense_hash_table_grow: Alloc fail");
    }

    /* Free old indices */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }
    free(dht->indices);

    /* Calculate new indices */
    dht->capacity /= DHT_DEFAULT_GROWTH_RATE;
    dht->indices = new_indices;
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
    /* Free old indices */
    for (unsigned int i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }

    /* Calculate new indices */
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
    srand(time(NULL));
    collision_count = 0;

    struct DenseHashTable *dht = NULL;
    if ((dht = malloc(sizeof(struct DenseHashTable))) == NULL) {
        return NULL;
    }

    dht->indices = NULL;
    dht->entries = NULL;

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
    // We don't return an error here in case dht==NULL.
    // Attempting to destroy a null pointer should not result in an error.
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

    /*
     * Check if there's already an entry with key *key.
     * Return with an error if yes.
     */
    const ResultOption resopt = dense_hash_table_lookup(dht, key);
    if (!resopt.is_ok) {
        return Err(resopt.error_code, resopt.error_message);
    }

    /*
     * Check if entry already exists.  Return an error if yes.
     */
    if (resopt.value.is_some) {
        return Err(ENTRY_ALREADY_EXISTS, "From `dense_hash_table_insert`: Entry with key already exists");
    }

    /*
     * Grow capacity if necessary.
     */
    if (dht->size + 1 > dht->capacity) {
        const Result res = s_dense_hash_table_grow(dht);
        if (!res.is_ok) {
            return res;
        }
    }

    /* 
     * Appropriate index is found.  Add the DHT entry to the list of entries.
     */
    struct DenseHashTableEntry *new_entries = NULL;

    if ((new_entries = realloc(dht->entries, (dht->size + 1) * sizeof(struct DenseHashTableEntry))) == NULL) {
        return Err(ALLOC_FAIL_ERR, "From `dense_hash_table_insert()`: `dht->entries` cannot be reallocated.");
    }
    /*
     * Newly allocated slot may have garbage data.  Clean up.
     */
    new_entries[dht->size].key = NULL;

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
    // Check if the input parameters are valid
    if (dht == NULL || key == NULL) {
        // If either parameter is NULL, return an error result with a message
        return Err_option(
                NULPTR_ERR,
                "From `dense_hash_table_lookup()`: params `key` or `dht` is/are NULL.");
    }

    int hash;

    // Calculate the hash value for the given key.  If hash calculation fails,
    // return an error result with the appropriate message
    const Result res = calculate_hash(key);
    if (res.is_ok) {
        hash = res.value;
    } else {
        return Err_option(res.error_code, res.error_message);
    }

    const unsigned int mask = dht->capacity;

    // Calculate the initial candidate index based on the hash value
    unsigned int candidate_idx = hash % mask;
    const unsigned int first_candidate = candidate_idx;

    // If the entry at the initial candidate index is NULL, the key is not present
    if (dht->indices[candidate_idx] == NULL) {
        return Ok_option(None());
    }

    unsigned int num_tries = 1;
    // Perform linear probing to find the key in the hash table
    do {
        if (dht->indices[candidate_idx] != NULL) {
            // If a non-NULL entry is found, compare keys
            if (strcmp(dht->entries[*dht->indices[candidate_idx]].key, key) == 0) {
                // If the keys match, return an option with the index of the found key
                return Ok_option(Some(candidate_idx));
            }
        }

        candidate_idx = s_update_idx(candidate_idx, mask, num_tries);
        num_tries++;
    } while (candidate_idx != first_candidate);

    // If the loop completes without finding the key, return an option indicating key not found
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


    /* Remove entries[indices[idx]] from the list */
    for (unsigned int i = *dht->indices[idx_in_indices]; i < dht->size - 1; i++) {
        const Result res2 = dense_hash_table_entry_set(&dht->entries[i], dht->entries[i + 1].key, dht->entries[i + 1].value);
        if (!res2.is_ok) {
            return Err_option(res2.error_code, res2.error_message);
        }
    }
    /* Destroy last element of entries */
    dense_hash_table_entry_destroy(&dht->entries[dht->size - 1]);

    /* Reset indices[idx] to NULL */
    if (dht->indices[idx_in_indices] != NULL) {
        free(dht->indices[idx_in_indices]);
        dht->indices[idx_in_indices] = NULL;
    }

    /* Reduce size now
     * ! Hint: Some functions below, such as `s_dense_hash_table_shrink`
     *         need dht->size to be realtime accurate.
     */
    dht->size--;

    struct DenseHashTableEntry *new_entries = NULL;
    if (dht->size && (new_entries = realloc(dht->entries, dht->size * sizeof(struct DenseHashTableEntry))) == NULL) {
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

    return Ok_option(Some(777));
}
