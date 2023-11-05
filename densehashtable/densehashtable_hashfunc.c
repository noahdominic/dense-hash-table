#include "densehashtable.h"

#define LEFT_SHIFT_BITS 5

int *calculate_hash(const char *key)
{
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