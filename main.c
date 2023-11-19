#include "densehashtable.h"

int
main()
{
  const char* names[] = {
    "Awesome Etiquette",
    "Misquoting Jesus",
    "Fire",
  };

  const int subscriber_counts[] = { 412098, 2345, 12908 };

  struct DenseHashTable* subscribers = dense_hash_table_init();

  for (int i = 0; i < 3; i++) {
    if (!dense_hash_table_insert(subscribers, names[i], subscriber_counts[i])
           .is_ok) {
      goto emergency_exit;
    }
  }

  dense_hash_table_print(subscribers);

emergency_exit:
  dense_hash_table_destroy(subscribers);
  return 0;
}