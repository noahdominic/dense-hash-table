#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "densehashtable.h"
#include "rustyc.h"

#define NUM_ITEMS 12
unsigned long long collision_count;

// Function to generate a random alphanumeric string of length between minLen
// and maxLen
void
generateRandomName(char* name, int minLen, int maxLen);


int
main(int argc, char *argv[])
{
  printf("%i\n", argc);
  if (argc < 2) {
    printf("Usage: %s <number>\n", argv[0]);
    exit(1);
  }

  char *endptr;
  long num_items = strtol(argv[1], &endptr, 10);

  // Check for conversion errors
  if (*endptr != '\0') {
    printf("Invalid input: %s\n", argv[1]);
    return 1;
  }

  Result res;
  ResultOption resopt;

  srand(time(NULL));

  struct DenseHashTable* dht = NULL;
  char name[13];
  char names[num_items][13];
  int value = 0;
  for (int i = 0; i < num_items; i++) {
    if ((dht = dense_hash_table_init()) == NULL) {
      continue;
    }

    for (int j = 0; j < i; j++) {
      generateRandomName(name, 5, 12);
      strcpy(names[j], name);
      value = rand();

      res = dense_hash_table_insert(dht, name, value);
      if (!(res).is_ok) {
        puts("Called from `main()`\n"
             "Trying to: Call `dense_hash_table_insert()`\n");
        printf("(j=%i): Error %i: %s\n", j, res.error_code, res.error_message);
        goto erase_all;
      }
    }


    for (int j = 0; j < i; j++) {
      resopt = dense_hash_table_remove(dht, names[j]);
      if (!(resopt).is_ok) {
        puts("Called from `main()`\n"
             "Trying to: Call `dense_hash_table_remove()`\n");
        printf(
          "(j=%i): Error %i: %s\n", j, resopt.error_code, resopt.error_message);
        goto erase_all;
      }
    }


    printf("%u, %lld\n", i, collision_count);

    dense_hash_table_destroy(dht);
  }

  //    for (int i = 0; i < NUM_ITEMS; i++) {
  //        Result res;
  //
  //        res = dense_hash_table_insert(dht, names[i], values[i]);
  //
  //        if (!res.is_ok) {
  //            printf("Error %i: %s", res.error_code, res.error_message);
  //            goto erase_all;
  //        }
  //
  ////        res = dense_hash_table_print(dht);
  ////        if (!res.is_ok) printf("Error %i: %s", res.error_code,
  /// res.error_message);
  //    }
  //
  //    {
  //        Result res;
  //
  //        res = dense_hash_table_insert(dht, names[4], values[4]);
  //
  //        if (!res.is_ok) {

erase_all:

  return 0;
}


void
generateRandomName(char* name, int minLen, int maxLen)
{
  static const char charset[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  int length = rand() % (maxLen - minLen + 1) + minLen;

  for (int i = 0; i < length; ++i) {
    int index = rand() % (sizeof(charset) - 1);
    name[i] = charset[index];
  }

  name[length] = '\0'; // Null-terminate the string
}
