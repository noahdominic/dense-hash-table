#include "densehashtable.h"

/*
int *dense_hash_table_lookup(DenseHashTable *dht, const char *key);
int *dense_hash_table_remove(DenseHashTable *dht, const char *key);
*/
DenseHashTable *dense_hash_table_init() {
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

int dense_hash_table_destroy(DenseHashTable *dht) {
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

int dense_hash_table_print(const DenseHashTable *dht) {
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

int dense_hash_table_insert(DenseHashTable *dht, const int value, const char *key) {
    if (dht == NULL || key == NULL) {
        return NULLPTR_ERROR;
    }

    unsigned int **new_indices;

    if (dht->size + 1 > dht->capacity) {
        new_indices = (unsigned int **) realloc(dht->indices, 2 * dht->capacity * sizeof(unsigned int *));

        if (new_indices == NULL) {
            return ALLOC_ERROR;
        }

        dht->indices = new_indices;
        dht->capacity *= 2;
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

    int a = dht->entries[dht->size].hash;

    unsigned int latest_index = a % dht->capacity;
    dht->indices[latest_index] = (unsigned int*) malloc(sizeof (int));
    *dht->indices[latest_index] = dht->size;

    dht->size++;

    return ALL_OK;
}
