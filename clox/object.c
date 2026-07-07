#include "object.h"

#include "memory.h"
#include "value.h"
#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

static ObjString *allocateString(const char *chars, int length) {
    size_t sz = sizeof(ObjString) + (length + 1) * sizeof(char);
    ObjString *string = (ObjString *)allocateObject(sz, OBJ_STRING);
    string->length = length;
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    return string;
}

// chars is heap-allocated using reallocate
ObjString *takeString(char *chars, int length) {
    ObjString *result = allocateString(chars, length);
    FREE_ARRAY(char, chars, length, 0);
    return result;
}

ObjString *copyString(const char *chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        default: fprintf(stderr, "printObject: unreachable"); exit(1);
    }
}
