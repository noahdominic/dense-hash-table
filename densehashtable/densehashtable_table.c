#include "densehashtable.h"

static int s_dense_hash_table_grow(DenseHashTable *dht)
{
    println("CALLED");
    unsigned int **new_indices;
    new_indices = (unsigned int **) realloc(dht->indices, 2 * dht->capacity * sizeof(unsigned int *));

    if (new_indices == NULL) {
        return ALLOC_ERROR;
    }

    dht->indices = new_indices;
    dht->capacity *= 2;
    return ALL_OK;
}

/*
int *dense_hash_table_lookup(DenseHashTable *dht, const char *key);
int *dense_hash_table_remove(DenseHashTable *dht, const char *key);
*/
DenseHashTable *dense_hash_table_init()
{
    DenseHashTable *dht = (DenseHashTable *) malloc(sizeof(DenseHashTable));

    dht->capacity = DHT_INIT_CAPACITY;
    dht->size = 0;

    dht->entries = (DenseHashTableEntry *) malloc(dht->size * sizeof(DenseHashTableEntry));
    if (dht->entries == NULL) {
        free(dht);
        dht = NULL;
        return NULL;
    }

    dht->indices = (unsigned int **) calloc(dht->capacity, sizeof(unsigned int *));
    if (dht->indices == NULL) {
        free(dht->entries);
        dht->entries = NULL;
        free(dht);
        dht = NULL;
        return NULL;
    }

    return dht;
}

int dense_hash_table_destroy(DenseHashTable *dht)
{
    if (dht == NULL) {
        return NULLPTR_ERROR;
    }

    if (dht->entries != NULL) {
        free(dht->entries);
        dht->entries = NULL;
    }

    if (dht->indices != NULL) {
        unsigned int i;
        for (i = 0; i < dht->capacity; i++) {
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
    return ALL_OK;
}

int dense_hash_table_print(const DenseHashTable *dht)
{
    if (dht == NULL) {
        println("Error in `dense_dht__print`: `dht` is NULL");
        return NULLPTR_ERROR;
    }

    if (dht->indices == NULL) {
        println("Error in `dense_dht__print`: `dht->indices` is NULL");
        return NULLPTR_ERROR;
    }

    if (dht->entries == NULL) {
        println("Error in `dense_dht__print`: `dht->entries` is NULL");
        return NULLPTR_ERROR;
    }

    int i;

    println("Size:%u\tCapacity:%u", dht->size, dht->capacity);

    printf("The contents of `indices`: ");
    for (i = 0; i < dht->capacity; i++) {
        if (dht->indices[i] == NULL) {
            printf("NUL...");
        } else {
            printf("%u...", *dht->indices[i]);
        }
    }
    println("");

    printf("The contents of `entries`: [\n");
    for (i = 0; i < dht->size; i++) {
        dense_hash_table_entry_print(&(dht->entries[i]));
    }
    println("]");
    return ALL_OK;
}

int dense_hash_table_insert(DenseHashTable *dht, const char *key, const int value)
{
    if (dht == NULL || key == NULL) {
        return NULLPTR_ERROR;
    }

    unsigned int candidate_idx;
    unsigned int mask = dht->capacity;

    /*
     * Create a new temporary dht entry...
     */
    DenseHashTableEntry candidate_dht_entry;
    int candidate_dht_entry_init_status = dense_hash_table_entry_set(&candidate_dht_entry, key, value);
    if (candidate_dht_entry_init_status != ALL_OK) {
        return candidate_dht_entry_init_status;
    }

    /*
     * Find the index for the dht entry...
     */
    candidate_idx = candidate_dht_entry.hash % mask;
    int i = 0;
    while (dht->indices[candidate_idx] != NULL && i <= dht->capacity) {
        println("Uh-oh! Hash conflict either because dht->indices[%u]=%u is not null or because %u is less than the dht->capacity of %u", candidate_idx, dht->indices[candidate_idx], i, dht->capacity);
        candidate_idx = (candidate_idx + 1) % mask;
        i++;
        println("%u", i);
        if (i >= dht->capacity) {
            s_dense_hash_table_grow(dht);
            mask = dht->capacity;
            candidate_idx = candidate_dht_entry.hash % mask;
        }
    }
    DenseHashTableEntry *new_entries;
    new_entries = (DenseHashTableEntry *) realloc(dht->entries, (dht->size + 1) * sizeof(DenseHashTableEntry));
    if (new_entries == NULL) {
        return ALLOC_ERROR;
    }
    dht->entries = new_entries;

    int dhte_set_error_code = dense_hash_table_entry_set(&(dht->entries[dht->size]), key, value);
    if (dhte_set_error_code != ALL_OK) {
        return dhte_set_error_code;
    }

    dht->indices[candidate_idx] = (unsigned int *) malloc(sizeof(int));
    *dht->indices[candidate_idx] = dht->size;

    dht->size++;

    return ALL_OK;
}
