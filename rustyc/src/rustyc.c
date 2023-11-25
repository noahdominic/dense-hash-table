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


#include "rustyc.h"


Option
Some(int value)
{
  Option option = { 1, value };
  return option;
}

Option
None()
{
  Option option = { 0, -666 }; // -666 is a sentinel number if is_some is 0.
  return option;
}


Result
Ok(int value)
{
  Result result = { 1, 0, value, NULL };
  return result;
}

Result
Ok_empty()
{
  Result result = { 1, 0, 0, NULL };
  return result;
}

Result
Err(int error_code, const char* error_message)
{
  Result result = { 0, error_code, -666, error_message };
  return result;
}

ResultOption
Ok_option(Option value)
{
  ResultOption result = { 1, 0, value, NULL };
  return result;
}

ResultOption
Err_option(int error_code, const char* error_message)
{
  ResultOption result = { 0, error_code, None(), error_message };
  return result;
}