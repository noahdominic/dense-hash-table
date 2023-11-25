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