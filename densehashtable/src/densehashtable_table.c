#include "densehashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLOT_STAT_EMPTY (-1)


static unsigned int
s_probing_func(const unsigned int candidate_idx,
               const unsigned int mask,
               const unsigned int num_attempts)
{
  // return (candidate_idx + 1) % mask; // Linear probing
  // return (candidate_idx + num_attempts * num_attempts) % mask; // Quadratic
  // return (candidate_idx + 3) % mask; // Linear probing
  return (candidate_idx + rand()) % mask; // Random
}


static Result
dense_hash_table_entry_dev_print(const struct DenseHashTableEntry* entry)
{
  if (entry == NULL) {
    puts("Called from `dense_hash_table_entry_print\n"
         "Trying to: Verify param `entry`\n");
    return Err(E_DHT_E_NULL, DhtErrorMessages[E_DHT_E_NULL]);
  }

  printf(
    "{key: %s\tvalue: %i\thash: %i}\n", entry->key, entry->value, entry->hash);

  return Ok_empty();
}


static Result
dense_hash_table_dev_print(const struct DenseHashTable* dht)
{
  if (dht == NULL) {
    puts("Called from `dense_hash_table_print`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  printf("Size:%u\nCapacity:%u\n", dht->size, dht->capacity);

  printf("The contents of `indices`: ");
  for (unsigned int i = 0; i < dht->capacity; i++) {
    if (dht->indices[i] < 0) {
      printf("NUL...");
    } else {
      printf("%u...", dht->indices[i]);
    }
  }
  printf("\n");

  // The code below will fail if `dht->entries` == NULL.
  // However, we are hoping that nothing goes wrong
  // because when nothing goes wrong `dht->entries` is only NULL
  // when dht->size = 0;
  printf("The contents of `entries`: [\n");
  for (unsigned int i = 0; i < dht->size; i++) {
    dense_hash_table_entry_dev_print(&dht->entries[i]);
  }
  printf("]\n");

  return Ok_empty();
}


static Result
s_dense_hash_table_register_to_indices_arr(const struct DenseHashTable* dht,
                                           const int hash,
                                           const int index_of_entry)
{
  if (dht == NULL) {
    puts("Called from: `s_dense_hash_table_register_to_indices_arr`\n"
         "Trying to: Verify param `dht`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }
  if (dht->indices == NULL) {
    puts("Called from: `s_dense_hash_table_register_to_indices_arr`\n"
         "Trying to: Verify param `dht->indices`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  // Find free slot in table
  const unsigned int mask = dht->capacity;
  unsigned int candidate_idx, num_attempts;

  candidate_idx = hash % mask;
  num_attempts = 1;

  // Probe the table until an available slot is found.
  // Presumably, the array right now is filled with indices and
  // SLOT_STAT_EMPTYs.

  //                             !   ↓↓↓ CAN LOOP FOREVER
  while (dht->indices[candidate_idx] != SLOT_STAT_EMPTY) {
    candidate_idx = s_probing_func(candidate_idx, mask, num_attempts);
    num_attempts++;
  }

  // After a free slot is found, set to `index_of_entry`
  dht->indices[candidate_idx] = index_of_entry;

  return Ok_empty();
}


static Result
s_dense_hash_table_refresh_indices(struct DenseHashTable* dht)
{
  if (dht == NULL) {
    puts("Called from: `s_dense_hash_table_refresh_indices()`\n"
         "Trying to: Verify param `dht`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  for (unsigned int i = 0; i < dht->capacity; i++) {
    dht->indices[i] = SLOT_STAT_EMPTY;
  }

  for (unsigned int i = 0; i < dht->size; i++) {
    const Result res =
      s_dense_hash_table_register_to_indices_arr(dht, dht->entries[i].hash, i);
    if (!res.is_ok) {
      puts("Called from: `s_dense_hash_table_refresh_indices`\n"
           "Trying to: Register `dht->entries[i].hash` and `i` to "
           "`dht->indices`\n");
      return res;
    }
  }
  return Ok_empty();
}


static Result
s_dense_hash_table_grow(struct DenseHashTable* dht)
{
  if (dht == NULL) {
    puts("Called from: `s_dense_hash_table_grow()`\n"
         "Trying to: Verify param `dht`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  // Create new indices array
  int* new_indices =
    calloc(dht->capacity * DHT_DEFAULT_GROWTH_RATE, sizeof(int));

  if (new_indices == NULL) {
    puts("Called from: `s_dense_hash_table_grow()`\n"
         "Trying to: Allocate `new_indices`\n");
    return Err(E_ALLOC_FAIL, DhtErrorMessages[E_ALLOC_FAIL]);
  }

  // Free old indices then assign new indices to old.
  FREE(dht->indices);
  dht->indices = new_indices;

  // The function `s_dense_hash_table_register_to_indices_arr` that
  // is called in function `s_dense_hash_table_refresh_indices`
  // _DEPENDS_ on `dht->capacity` being correct.
  //
  // Update dht->capacity now.
  dht->capacity *= DHT_DEFAULT_GROWTH_RATE;

  const Result res = s_dense_hash_table_refresh_indices(dht);
  if (!res.is_ok) {
    puts("Called from: `s_dense_hash_table_grow()`\n"
         "Trying to: Call `s_dense_hash_table_refresh_indices()`\n");
    return res;
  }

  return Ok_empty();
}


static Result
s_dense_hash_table_shrink(struct DenseHashTable* dht)
{
  if (dht == NULL) {
    puts("Called from: `s_dense_hash_table_shrink()`\n"
         "Trying to: Verify param `dht`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  // Create new indices array
  int* new_indices =
    calloc(dht->capacity / DHT_DEFAULT_GROWTH_RATE, sizeof(int));

  if (new_indices == NULL) {
    puts("Called from `s_dense_hash_table_shrink`\n");
    return Err(E_ALLOC_FAIL, DhtErrorMessages[E_ALLOC_FAIL]);
  }

  // Free old indices then assign new indices to old.
  FREE(dht->indices);
  dht->indices = new_indices;

  // The function `s_dense_hash_table_register_to_indices_arr` that
  // is called in function `s_dense_hash_table_refresh_indices`
  // _DEPENDS_ on `dht->capacity` being correct.
  //
  // Update dht->capacity now.
  dht->capacity /= DHT_DEFAULT_GROWTH_RATE;

  const Result res = s_dense_hash_table_refresh_indices(dht);
  if (!res.is_ok) {
    puts("Called from `s_dense_hash_table_grow`\n");
    return res;
  }

  return Ok_empty();
}


struct DenseHashTable*
dense_hash_table_init()
{
  struct DenseHashTable* dht = malloc(sizeof(struct DenseHashTable));
  if (dht == NULL) {
    return NULL;
  }

  dht->indices = calloc(DHT_INIT_CAPACITY, sizeof(int));
  if (dht->indices == NULL) {
    FREE(dht);
    return NULL;
  }

  for (int i = 0; i < DHT_INIT_CAPACITY; i++) {
    dht->indices[i] = SLOT_STAT_EMPTY;
  }
  dht->entries = NULL;
  dht->capacity = DHT_INIT_CAPACITY;
  dht->size = 0;

  return dht;
}


Result
dense_hash_table_destroy(struct DenseHashTable* dht)
{
  if (dht == NULL) {
    return Ok_empty();
  }

  if (dht->entries != NULL) {
    for (unsigned int i = 0; i < dht->size; i++) {
      FREE(dht->entries[i].key);
      dht->entries[i].hash = 0;
      dht->entries[i].value = 0;
    }
  }

  FREE(dht->entries);
  FREE(dht->indices);
  FREE(dht);
  return Ok_empty();
}


Result
dense_hash_table_print(const struct DenseHashTable* dht)
{
  if (dht == NULL) {
    puts("Called from `dense_hash_table_print`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }

  // The code below will fail if `dht->entries` == NULL.
  // However, we are hoping that nothing goes wrong
  // because when nothing goes wrong `dht->entries` is only NULL
  // when dht->size = 0;
  printf("{\n");
  for (unsigned int i = 0; i < dht->size; i++) {
    dense_hash_table_entry_print(&dht->entries[i]);
  }
  printf("}\n");

  return Ok_empty();
}


Result
dense_hash_table_insert(struct DenseHashTable* dht,
                        const char* key,
                        const int value)
{
  if (dht == NULL) {
    puts("Called from: `dense_hash_table_insert()`\n"
         "Trying to: Verify param `dht`\n");
    return Err(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }
  if (key == NULL) {
    puts("Called from: `dense_hash_table_insert()`\n"
         "Trying to: Verify param `key`\n");
    return Err(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  // Check if there's already an entry with key *key.
  // Return with a Result<None> if yes.
  const ResultOption res_opt = dense_hash_table_lookup(dht, key);
  if (!res_opt.is_ok) {
    puts("Called from: `dense_hash_table_insert()`\n"
         "Trying to: Call `dense_hash_table_lookup()`\n");
    return Err(res_opt.error_code, res_opt.error_message);
  }
  if (res_opt.value.is_some) {
    Ok_option(None());
  }

  // Grow capacity if necessary.
  // Return with an error if there's a problem with growing the table.
  if (dht->size + 1 > dht->capacity / DHT_DEFAULT_GROWTH_RATE) {
    const Result res = s_dense_hash_table_grow(dht);
    if (!res.is_ok) {
      puts("Called from: `dense_hash_table_insert()`\n"
           "Trying to: Call `s_dense_hash_table_grow()`\n");
      return res;
    }
  }

  struct DenseHashTableEntry* temp_entries =
    calloc(dht->capacity + 1, sizeof(struct DenseHashTableEntry));
  if (temp_entries == NULL) {
    puts("Called from: `dense_hash_table_insert()`\n"
         "Trying to: Allocate `temp_entries`\n");
    return Err(E_ALLOC_FAIL, DhtErrorMessages[E_ALLOC_FAIL]);
  }

  for (int i = 0; i < dht->size; i++) {
    Result res = dense_hash_table_entry_set(
      &temp_entries[i], dht->entries[i].key, dht->entries[i].value);
    if (!res.is_ok) {
      puts("Called from: `dense_hash_table_insert()`\n"
           "Trying to: Copy dht->entries[i] to temp_entries[i] "
           "with `dense_hash_table_entry_set()`\n");
      return res;
    }
    FREE(dht->entries[i].key);
    dht->entries[i].hash = 0;
    dht->entries[i].value = 0;
  }
  FREE(dht->entries);

  Result res = dense_hash_table_entry_set(&temp_entries[dht->size], key, value);
  if (!res.is_ok) {
    puts("Called from: `dense_hash_table_insert()`\n"
         "Trying to: Set temp_entries[dht->size] to latest entry"
         "with `dense_hash_table_entry_set()`\n");
    return res;
  }

  dht->entries = temp_entries;

  res = s_dense_hash_table_register_to_indices_arr(
    dht, dht->entries[dht->size].hash, dht->size);
  if (!res.is_ok) {
    return res;
  }

  dht->size++;

  return Ok_empty();
}


ResultOption
dense_hash_table_remove(struct DenseHashTable* dht, const char* key)
{
  if (dht == NULL) {
    puts("Called from: `dense_hash_table_remove()`\n"
         "Trying to: Verify param `dht`\n");
    return Err_option(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }
  if (key == NULL) {
    puts("Called from: `dense_hash_table_remove()`\n"
         "Trying to: Verify param `key`\n");
    return Err_option(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  // Get index of key
  const ResultOption res = dense_hash_table_lookup(dht, key);
  if (!res.is_ok) {
    puts("Called from `dense_hash_table_remove()`\n"
         "Trying to: Call `dense_hash_table_lookup()`\n");
    return res;
  } else if (!res.value.is_some) { // i.e., if there is no error but the value
                                   // is blank
    return Ok_option(None());
  }
  const int idx_in_indices = res.value.value;

  // Remove item at idx_in_indices by
  // copying the next entry's values and looping until the end of array
  for (unsigned int i = dht->indices[idx_in_indices]; i < dht->size - 1; i++) {
    const Result res2 = dense_hash_table_entry_set(
      &dht->entries[i], dht->entries[i + 1].key, dht->entries[i + 1].value);
    if (!res2.is_ok) {
      puts("Called from `dense_hash_table_remove()`\n"
           "Trying to: Copy dht->entries[i+1]'s elements to dht->entries[i] "
           "with `dense_hash_table_entry_set()`\n");
      return Err_option(res2.error_code, res.error_message);
    }
  }

  /* Cleanup last element of entries */
  dense_hash_table_entry_cleanup(&dht->entries[dht->size - 1]);

  // Create new entries array with one less entry
  struct DenseHashTableEntry* new_entries =
    calloc(dht->size - 1, sizeof(struct DenseHashTableEntry));
  if (new_entries == NULL) {
    puts("Called from `dense_hash_table_remove()`.\n"
         "Trying to: Alloc `new_entries`.\n");
    return Err_option(E_ALLOC_FAIL, DhtErrorMessages[E_ALLOC_FAIL]);
  }

  // Copy old entries to the new entries
  for (int i = 0; i < dht->size - 1; i++) {
    const Result res2 = dense_hash_table_entry_set(
      &new_entries[i], dht->entries[i].key, dht->entries[i].value);
    if (!res2.is_ok) {
      puts("Called from `dense_hash_table_remove()`.\n"
           "Trying to: Copy `dht->entries` with "
           "`dense_hash_table_entry_set()`.\n");
      return Err_option(res2.error_code, res.error_message);
    }
    dense_hash_table_entry_cleanup(&dht->entries[i]);
  }

  FREE(dht->entries);
  dht->entries = new_entries;

  /* Reduce size now.
   * ! Hint: Some functions below, such as `s_dense_hash_table_shrink`
   *         need dht->size to be realtime accurate.
   */
  dht->size--;

  // Check if indices needs resizing
  if (dht->size <=
        dht->capacity / (DHT_DEFAULT_GROWTH_RATE * DHT_DEFAULT_GROWTH_RATE) &&
      dht->capacity > DHT_INIT_CAPACITY) {
    const Result res2 = s_dense_hash_table_shrink(dht);
    if (!res2.is_ok) {
      puts("Called from `dense_hash_table_remove()`.\n"
           "Trying to: Call `s_dense_hash_table_shrink()`.\n");
      return Err_option(res2.error_code, res2.error_message);
    }
  }

  const Result res2 = s_dense_hash_table_refresh_indices(dht);
  if (!res2.is_ok) {
    puts("Called from `dense_hash_table_remove()`.\n"
         "Trying to: Call `s_dense_hash_table_refresh_indices()`.\n");
    return Err_option(res2.error_code, res2.error_message);
  }

  return Ok_option(Some(idx_in_indices));
}


ResultOption
dense_hash_table_lookup(const struct DenseHashTable* dht, const char* key)
{
  if (dht == NULL) {
    puts("Called from: `dense_hash_table_lookup()`\n"
         "Trying to: Verify param `dht`\n");
    return Err_option(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
  }
  if (key == NULL) {
    puts("Called from: `dense_hash_table_lookup()`\n"
         "Trying to: Verify param `key`\n");
    return Err_option(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  int hash;
  // Calculate the hash value for the given key.  If hash calculation fails,
  // return an error result with the appropriate message
  const Result res = calculate_hash(key);
  if (res.is_ok) {
    hash = res.value;
  } else {
    puts("Called from: `dense_hash_table_lookup()`\n"
         "Trying to: Call `calculate_hash()`\n");
    return Err_option(res.error_code, res.error_message);
  }

  const unsigned int mask = dht->capacity;

  // Calculate the initial candidate index based on the hash value
  unsigned int candidate_idx = hash % mask;
  const unsigned int first_candidate = candidate_idx;

  // If the entry at the initial candidate index is SLOT_STAT_EMPTY,
  // the key is not present.
  if (dht->indices[candidate_idx] == SLOT_STAT_EMPTY) {
    return Ok_option(None());
  }

  // Perform probing to find the key in the hash table
  unsigned int num_tries = 1;
  do {
    if (dht->indices[candidate_idx] != SLOT_STAT_EMPTY) {
      // If a non-SLOT_STAT_EMPTY entry is found, compare keys
      if (dht->entries[dht->indices[candidate_idx]].key == NULL) {
        printf("i = %i\n", candidate_idx);
        puts("Called from: `dense_hash_table_lookup()`\n"
             "Trying to: Verify "
             "`dht->entries[dht->indices[candidate_idx]].key`\n");
        return Err_option(E_DHT_NULL, DhtErrorMessages[E_DHT_NULL]);
      }
      if (strcmp(dht->entries[dht->indices[candidate_idx]].key, key) == 0) {
        // If the keys match, return an option with the index of the found key
        return Ok_option(Some(candidate_idx));
      }
    }

    candidate_idx = s_probing_func(candidate_idx, mask, num_tries);
    num_tries++;
  } while (candidate_idx != first_candidate);

  // If the loop completes without finding the key, return an option indicating
  // not found.
  return Ok_option(None());
}
