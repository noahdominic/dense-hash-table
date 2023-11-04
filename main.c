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
    error = dense_hash_table_insert(dht, "KL", 69);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Maria", 33);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Lukas", 26);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Abdul", 06);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Christopher", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Guy", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Kian", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Matthew", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Mark", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Luke", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "John", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Adam", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Seth", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Enoch", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Malachi", 36);
    error += dense_hash_table_print(dht);

    println("-------");
    error += dense_hash_table_insert(dht, "Noah", 36);
    error += dense_hash_table_print(dht);
    error += dense_hash_table_destroy(dht);

    return error;
}