#include <stdio.h>
#include <stdlib.h>

#include "densehashtable.h"
#include "rustyc.h"

int main() {
    const char *name = "Zendaya";

    Result result = calculate_hash(name);

    if (result.is_ok) {
        printf("The hash of \"%s\" is %i\n", name, result.value);
    } else {
        printf("Error %i.... %s\n", result.error_code, result.error_message);
        return result.error_code;
    }


    return 0;
}