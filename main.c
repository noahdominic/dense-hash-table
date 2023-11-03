#include "noahdmncio/noahdmncio.h"
#include "densehashtable/densehashtable.h"

int main() {
    const char* name = "Kyle";
    println("Hello, %s", name);

    DenseHashTableEntry *dhte = dht_entry_init(name, 69);
    int error = dht_entry_print(dhte);
    error += dht_entry_destroy(dhte);

    return error;
}