#pragma once

#define NULL 0

/*
 * A naive C implementation of Rust's Option<int> feature.
 * Use for instances where the return value of a function _may_ not be present.
 * To be clear, this is not appropriate for error cases;
 * it is appropriate only for successful functions but still has no return value.
 *
 * For instance, this would be appropriate for a function f() that accepts
 * `int[] arr` and `int search`.  There may be an instance where `search` is not
 * in `arr` (e.g.  `arr` = `{5, 6, 7, 8, 10}` and `search` = 9).
 */
typedef struct {
    int is_some;
    int value;
} Option;

Option Some(int value);

Option None();

/*
 * A naive C implementation of Rust's Result<int, Error> feature.
 * Use for instances where the return value of a function can either be an integer
 * or an error code.
 * This is appropriate for functions that can return a value or an error condition.
 *
 * For instance, this would be appropriate for a function that reads a file
 * and returns either the content of the file as an integer or an error message.
 */

typedef struct {
    int is_ok;
    int error_code;
    int value;
    const char *error_message;
} Result;


Result Ok(int value);

Result Ok_empty();

Result Err(int error_code, const char *error_message);
