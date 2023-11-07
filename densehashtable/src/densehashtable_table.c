#include "densehashtable.h"

static Result s_dense_hash_table_register_entry(
        struct DenseHashTable *dht,
        const int hash,
        const unsigned int index_of_entry)
{
    srand(clock());

    unsigned int mask = dht->capacity;

    unsigned int candidate_idx = hash % mask;

    while (dht->indices[candidate_idx] != NULL) {
        candidate_idx = (rand()) % mask;
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
    /*
     * Free old indices
     */
    for (unsigned int i = 0; i < dht->size; i++) {
        if (dht->indices[i] != NULL) {
            free(dht->indices[i]);
            dht->indices[i] = NULL;
        }
    }
    free(dht->indices);
    /*
     * Calculate new indices
     */
    dht->capacity *= DHT_DEFAULT_GROWTH_RATE;
    dht->indices = calloc(dht->capacity, sizeof(unsigned int *));
    for (unsigned int i = 0; i < dht->size; i++) {
        Result res = s_dense_hash_table_register_entry(dht, dht->entries[i].hash, i);
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
        dense_hash_table_entry_print(&(dht->entries[i]));
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
        Result res;
        if (!(res = s_dense_hash_table_grow(dht)).is_ok) {
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

    Result res = dense_hash_table_entry_set(&(dht->entries[dht->size]), key, value);
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