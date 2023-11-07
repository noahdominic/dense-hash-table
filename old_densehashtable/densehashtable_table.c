#include "densehashtable.h"

static void dense_hash_table_register_entry(struct DenseHashTable *dht, const int hash, const unsigned int index_of_entry)
{
    srand((unsigned int) clock());

    unsigned int mask = dht->capacity;

    unsigned int candidate_idx = hash % mask;

    while (dht->indices[candidate_idx] != NULL) {
        candidate_idx = (rand()) % mask;
    }

    dht->indices[candidate_idx] = (unsigned int *) malloc(sizeof(int));
    *dht->indices[candidate_idx] = index_of_entry;
}

static int s_dense_hash_table_grow(struct DenseHashTable *dht)
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
    dht->capacity *= DHT_DEFAULT_GROWTH_CONST;
    dht->indices = (unsigned int **) calloc(dht->capacity, sizeof(unsigned int *));
    for (unsigned int i = 0; i < dht->size; i++) {
        dense_hash_table_register_entry(dht, dht->entries[i].hash, i);
    }

    return ALL_OK;
}

/*
int *dense_hash_table_lookup(struct DenseHashTable *dht, const char *key);
int *dense_hash_table_remove(struct DenseHashTable *dht, const char *key);
*/
struct DenseHashTable *dense_hash_table_init()
{
    struct DenseHashTable *dht = (struct DenseHashTable *) malloc(sizeof(struct DenseHashTable));
    if (dht == NULL) {
        return NULL;
    }

    dht->indices = (unsigned int **) calloc(DHT_INIT_CAPACITY, sizeof(unsigned int *));
    if (dht->indices == NULL) {
        free(dht);
        dht = NULL;
        return NULL;
    }

    dht->capacity = DHT_INIT_CAPACITY;
    dht->size = 0;

    return dht;
}

int dense_hash_table_destroy(struct DenseHashTable *dht)
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

    return ALL_OK;
}

int dense_hash_table_print(const struct DenseHashTable *dht)
{
    if (dht == NULL || dht->indices == NULL || dht->entries == NULL) {
        return NULLPTR_ERROR;
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

    printf("The contents of `entries`: [\n");
    for (unsigned int i = 0; i < dht->size; i++) {
        dense_hash_table_entry_print(&(dht->entries[i]));
    }
    printf("]\n");
    return ALL_OK;
}

int dense_hash_table_insert(struct DenseHashTable *dht, const char *key, const int value)
{
    /*
        This flow seems to be iffy.  There's too many things that can go wrong.
        Would be preferable to change this such that a duplicate `dht` is made
        so that any errors that can happen will not affect the original.  Only
        when everything goes perfectly will the new table 
        be moved to the original.
    */
    if (dht == NULL || key == NULL) {
        return NULLPTR_ERROR;
    }

    if (dht->size + 1 > dht->capacity) {
        s_dense_hash_table_grow(dht);
    }

    /* 
     * Appropriate index is found. Now let's add the DHT entry to the list of entries.
     */
    struct DenseHashTableEntry *new_entries = (struct DenseHashTableEntry *) realloc(dht->entries, (dht->size + 1) * sizeof(struct DenseHashTableEntry));
    if (new_entries == NULL) {
        return ALLOC_ERROR;
    }
    dht->entries = new_entries;

    int dhte_set_error_code = dense_hash_table_entry_set(&(dht->entries[dht->size]), key, value);
    if (dhte_set_error_code != ALL_OK) {
        return dhte_set_error_code;
    }

    dense_hash_table_register_entry(dht, dht->entries[dht->size].hash, dht->size);

    dht->size++;

    return ALL_OK;
}