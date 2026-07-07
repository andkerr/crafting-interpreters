#include "memory.h"

#include <stdlib.h>

void *reallocate(void *data, size_t oldSize, size_t newSize) {
    (void) oldSize;

    if (newSize == 0) {
        free(data);
        return NULL;
    }

    void *result = realloc(data, newSize);
    if (result == NULL) {
        exit(1);
    }
    
    return result;
}
