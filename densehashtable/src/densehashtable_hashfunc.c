/*
 * Copyright 2023, 2024  Noah Dominic Miranda Silvio
 *
 * Licensed under the European Union Public Licence v1.2
 */


#include "densehashtable.h"

#include <string.h>

#define LEFT_SHIFT_BIT_AMOUNT 5


Result
calculate_hash(const char* key)
{
  if (key == NULL) {
    return Err(E_FUNC_PARAM_NULL, DhtErrorMessages[E_FUNC_PARAM_NULL]);
  }

  int hash = 0;

  unsigned int len = strlen(key);

  for (int i = 0; i < len; i++) {
    hash = (hash << LEFT_SHIFT_BIT_AMOUNT) - hash + key[i];
  }

  return Ok(hash);
}