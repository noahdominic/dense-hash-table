#include "densehashtable.h"

#define LEFT_SHIFT_BIT_AMOUNT 5

Result calculate_hash(const char *key)
{
    if (key == NULL) {
        return Err(NULPTR_ERR, "`calculate_hash()` param `key` is NULL.");
    }

    int hash = 0;

    unsigned int len = strlen(key);

    for (int i = 0; i < len; i++) {
        hash = (hash << LEFT_SHIFT_BIT_AMOUNT) - hash + key[i];
    }

    return Ok(hash);
}