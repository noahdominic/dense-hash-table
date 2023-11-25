/*
 * Copyright 2023 Noah Dominic Miranda Silvio
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the European Union Public License version 1.2,
 * as published by the European Commission.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * European Union Public Licence for more details.
 *
 * You should have received a copy of the European Union Public Licence
 * along with this program. If not, see
 * <https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12>.
 */


#include "densehashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void
dense_hash_table_entry_cleanup(struct DenseHashTableEntry* entry)
{
  if (entry == NULL) {
    return;
  }

  free(entry->key);
  entry->key = NULL;

  entry->value = 0;
  entry->hash = 0;
}


Result
dense_hash_table_entry_set(struct DenseHashTableEntry* entry,
                           const char* key,
                           const int value)
{
  int hash;

  if (entry == NULL) {
    puts("Called from `dense_hash_table_entry_set\n"
         "Trying to: Verify param `entry`\n");
    return Err(E_DHT_E_NULL, DhtErrorMessages[E_DHT_E_NULL]);
  }

  if (key == NULL) {
    puts("Called from: `dense_hash_table_entry_set()`\n"
         "Trying to: Verify param `key`\n");
    return Err(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  const Result res = calculate_hash(key);
  if (res.is_ok) {
    hash = res.value;
  } else {
    puts("Called from: `dense_hash_table_entry_set()`\n"
         "Trying to: Call `calculate_hash()`\n");
    return Err(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  // This `free` block SHOULDN'T be necessary.   I have no idea why sometimes
  // entry->key isn't NULL.
  FREE(entry->key);

  if ((entry->key = malloc(strlen(key) + 1)) == NULL) {
    puts("Called from: `dense_hash_table_entry_set()`\n"
         "Trying to: Alloc `entry->key`\n");
    return Err(E_ALLOC_FAIL, DhtErrorMessages[E_ALLOC_FAIL]);
  }

  strcpy(entry->key, key);
  entry->value = value;
  entry->hash = hash;

  return Ok_empty();
}


Result
dense_hash_table_entry_print(const struct DenseHashTableEntry* entry)
{
  if (entry == NULL) {
    puts("Called from `dense_hash_table_entry_print\n"
         "Trying to: Verify param `entry`\n");
    return Err(E_DHT_E_NULL, DhtErrorMessages[E_DHT_E_NULL]);
  }

  printf("\t\"%s\": %i,\n", entry->key, entry->value);

  return Ok_empty();
}