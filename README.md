# Dense Hash Table

A more memory-efficient implementation of a hash table in C.

## Usage

### Installation

To include `densehashtable` and `rustyc` as libraries in your own CMake project, follow these steps:

1. Clone this repository: `git clone https://github.com/noahdominic/dense-hash-table.git`
1. Copy the `densehashtable` and `rustyc` directories into your project's directory structure.
1. In your project's main CMakeLists.txt file, add the following lines:

    ```cmake
    add_subdirectory(densehashtable)
    add_subdirectory(rustyc)

    target_link_libraries(your_target densehashtable rustyc)
    ```

    Replace `your_target` with the name of your CMake target.

1. Ensure that your project includes the necessary header files from `densehashtable` and `rustyc`.

### Example

Here's a simple example demonstrating how to use the dense hash table:

```c
#include <stdio.h>
#include "densehashtable.h"

int main() {
    // Initialise a hash table with a capacity of 8 elements
    struct DenseHashTable *hash_table = dense_hash_table_init();

    // Insert key-value pairs
    if (!dense_hash_table_insert(hash_table, "key1", 123).is_ok){
        // Handle insertion failures here...
        goto emergency_exit;
    }
    if (!dense_hash_table_insert(hash_table, "key2", 4635).is_ok){
        // Handle insertion failures here...
        goto emergency_exit;
    }

    // Retrieve values by keys
    ResultOption result_handler = dense_hash_table_lookup(hash_table, "key1");
    // Check for errors
    if (!result_handler.is_ok) {
        goto emergency_exit;
    } 
    // Check if key is found
    if (!result_handler.value.is_some) {
        printf("Not found!\n");
    }
    const char value1 = result_handler.value.value;

    result_handler = dense_hash_table_lookup(hash_table, "key2");
    // Check for errors
    if (!result_handler.is_ok) {
        goto emergency_exit;
    } 
    // Check if key is found
    if (!result_handler.value.is_some) {
        printf("Not found!\n");
    }
    const char value2 = result_handler.value.value;

    printf("Value for key1: %s\n", value1);
    printf("Value for key2: %s\n", value2);

emergency_exit:
    // Clean up memory
    dense_hash_table_destroy(hash_table);

    return 0;
}

```

## Limitations

At present, the dense hash table implementation provided in this repository has the following limitations:

- **Data Type Restriction:** The hash table is designed to handle only `int` data types for keys and values.  Using other data types may result in unexpected behavior.

- **Dependency on a rudimentary replication of Rust's `Result` and `Option` types:** The library includes basic functionalities inspired by Rust's `Result` and `Option` types.  Usage and behavior might differ from the original Rust features, and the addition of such may not be idiomatic C.

## Contribution

Contributions to enhance the implementation, add features, or fix issues are welcome!  Please fork this repository and submit pull requests to contribute.

## Licence

This project is licensed under the EU Public Licence v. 1.2. Please see the [LICENCE](LICENCE) file for details.
