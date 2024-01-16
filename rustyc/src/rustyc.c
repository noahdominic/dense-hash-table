/*
 * Copyright 2023, 2024  Noah Dominic Miranda Silvio
 *
 * Licensed under the European Union Public Licence v1.2
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