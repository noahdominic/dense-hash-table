/*
 * Copyright 2023, 2024  Noah Dominic Miranda Silvio
 *
 * Licensed under the European Union Public Licence v1.2
 */


#include "densehashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char*
generate_random_string()
{

  int length = rand() % 13 + 4; // Generates a random length between 4 and 16
  char* characters =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int characters_count = 62; // Total alphanumeric characters

  char* random_string = (char*)malloc(
    (length + 1) * sizeof(char)); // Allocate memory for the string
  if (random_string == NULL) {
    return NULL; // Memory allocation failed
  }

  for (int i = 0; i < length; ++i) {
    random_string[i] =
      characters[rand() % characters_count]; // Choose a random character
  }
  random_string[length] = '\0'; // Null-terminate the string

  return random_string;
}

int
main(int argc, char* argv[])
{
  srand((unsigned int)time(NULL));
  int error_code = 0;
  if (argc > 1) {
    if (strcmp(argv[1], "benchmark") == 0) {
      if (argc != 3) {
  puts("Error: `benchmark` requires a number input.\n");
  return -1;
      }
      const int number_of_items = atoi(argv[2]);

      int random_ints[number_of_items];

      for (int i = 0; i < number_of_items; i++) {
  random_ints[i] = rand();
      }

      char** random_strings = calloc(number_of_items, sizeof(char*));
      for (int i = 0; i < number_of_items; i++) {
  random_strings[i] = generate_random_string();
      }

      struct DenseHashTable* dht = dense_hash_table_init();
      if (dht == NULL) {
  error_code = -1;
  goto emergency_exit_benchmark;
      }

      for (int i = 0; i < number_of_items; i++) {
  if (!dense_hash_table_insert(dht, random_strings[i], random_ints[i])
         .is_ok) {
    error_code = -1;
    goto emergency_exit_benchmark;
  }
      }

    emergency_exit_benchmark:
      dense_hash_table_destroy(dht);

      for (int i = 0; i < number_of_items; i++) {
  free(random_strings[i]);
      }

      free(random_strings);

      return 0;
    }
  }

  const char* names[] = {
    "Awesome Etiquette",
    "Misquoting Jesus",
    "Fire",
  };

  const int subscriber_counts[] = { 412098, 2345, 12908 };

  struct DenseHashTable* subscribers = dense_hash_table_init();
  if (subscribers == NULL) {
    error_code = -1;
    goto emergency_exit;
  }

  for (int i = 0; i < 3; i++) {
    if (!dense_hash_table_insert(subscribers, names[i], subscriber_counts[i])
     .is_ok) {
      error_code = -1;
      goto emergency_exit;
    }
  }

  const char* lookup_terms[] = {
    "Misquoting Jesus",  // Is in table
    "Lorem Ipsum"    // Is NOT in table
  };
  
  for (int i; i < 2; i++) {
    printf("Looking for %s... ", lookup_terms[i]);
  
  const ResultOption valuehandler = dense_hash_table_lookup(subscribers, lookup_terms[i]);

  if (!valuehandler.is_ok) {
    error_code = -1;
    goto emergency_exit;
  }

  if (!valuehandler.value.is_some) {
    printf("%s is not in dht!\n", lookup_terms[i]);
  } else {
    const int value = valuehandler.value.value;

    printf("The value of %s is %i.\n", lookup_terms[i], value);
  }
  }


  dense_hash_table_print(subscribers);

emergency_exit:
  dense_hash_table_destroy(subscribers);
  return error_code;
}
