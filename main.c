#include "densehashtable/densehashtable.h"
#include "noahdmncio/noahdmncio.h"

int main() {
    const char *name = "Kyle";
    println("Hello, %s", name);

    //    DenseHashTableEntry *dhte = dense_hash_table_entry_init(name, 69);
    //    int error = dense_hash_table_entry_print(dhte);
    //    error += dense_hash_table_entry_destroy(dhte);

    DenseHashTable *dht = dense_hash_table_init();
    int error;
    println("-------");
    error = dense_hash_table_insert(dht, 69, "KL");
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, 33, "Maria");
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, 26, "Lukas");
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, 06, "Abdul");
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, 36, "Christopher");
    error += dense_hash_table_print(dht);
    error += dense_hash_table_destroy(dht);

    return error;
}