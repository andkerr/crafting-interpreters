#include "memory.h"

#include "vm.h"

#include <stdlib.h>

static void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_FUNCTION: {
            ObjFunction *function = (ObjFunction *)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_NATIVE: {
            FREE(ObjNative, object);
            break;
        }
        case OBJ_CLOSURE: {
            ObjClosure *closure = (ObjClosure *)object;
            FREE_ARRAY(ObjUpvalue *, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, object);
            break;
        }
        case OBJ_UPVALUE: {
            FREE(ObjUpvalue, object);
            break;
        }
        case OBJ_STRING: {
            // TODO: This is technically incorrect now that ObjString uses
            //       a VLA for its character buffer, since FREE assumes all
            //       ObjStrings occupy sizeof(ObjString) bytes. We are only
            //       saved by reallcate() always ignoring the "old size" arg
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
