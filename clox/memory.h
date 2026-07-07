#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, data, oldCount, newCount) \
    (type *)reallocate(data, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE_ARRAY(type, data, oldCount) \
    reallocate(data, sizeof(type) * (oldCount), 0)

void *reallocate(void *data, size_t oldSize, size_t newSize);

#endif
