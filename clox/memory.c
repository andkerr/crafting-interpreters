#include "memory.h"

#include "vm.h"

#include <stdlib.h>

static void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_STRING: {
            FREE(ObjString, object);
            break;
        }
    }
}

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

void freeObjects() {
    Obj *object = vm.objects;
    while (object != NULL) {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}
