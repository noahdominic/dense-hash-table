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

int
main()
{
  int error_code = 0;
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

  ResultOption valuehandler = dense_hash_table_lookup(subscribers, "jkjgh");

  if (!valuehandler.is_ok) {
    error_code = -1;
    goto emergency_exit;
  }

  if (!valuehandler.value.is_some) {
    printf("Fire is not in dht!\n");
  } else {
    const int value = valuehandler.value.value;

    printf("The value of %s is %i.\n", "Fire", value);
  }


  dense_hash_table_print(subscribers);

emergency_exit:
  dense_hash_table_destroy(subscribers);
  return error_code;
}