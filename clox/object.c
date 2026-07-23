#include "object.h"

#include "memory.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATE_OBJ(type, objectType) \
    (type *)allocateObject(sizeof(type), objectType)

static void printFunction(ObjFunction *function) {
    printf("<fn %s>", function->name == NULL
        ? "<script>" : function->name->chars);
}

static uint32_t hashString(const char *key, int length) {
    uint32_t hash = 2166136361u;
    for (int i = 0; i < length; ++i) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static ObjString *allocateString(const char *chars, int length,
                                 uint32_t hash) {
    // compute size manually, since 'chars' is a VLA and we probably need
    // more memory than sizeof(ObjString)
    size_t sz = sizeof(ObjString) + (length + 1) * sizeof(char);
    ObjString *string = (ObjString *)allocateObject(sz, OBJ_STRING);
    string->length = length;
    string->hash = hash;
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    tableSet(&vm.strings, string, NIL_VAL);
    return string;
}

ObjFunction *newFunction() {
    ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
}

ObjNative *newNative(NativeFn function, int arity) {
    ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    native->arity = arity;
    return native;
}

ObjUpvalue *newUpvalue(Value *slot) {
    ObjUpvalue *upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->location = slot;
    return upvalue;
}

ObjClosure *newClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue *, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; ++i) {
        upvalues[i] = NULL;
    }

    ObjClosure *closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

// precondition: chars is heap-allocated using reallocate
ObjString *takeString(char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length,
                                          hash);
    ObjString *result;
    if (interned != NULL) {
        result = interned;
    }
    else {
        result = allocateString(chars, length, hash);
    }

    FREE_ARRAY(char, chars, length);
    return result;
}

ObjString *copyString(const char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length,
                                          hash);
    if (interned != NULL) return interned;

    return allocateString(chars, length, hash);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            printFunction(AS_FUNCTION(value));
            break;
        case OBJ_NATIVE:
            printf("<native fn>");
            break;
        case OBJ_CLOSURE:
            printFunction(AS_CLOSURE(value)->function);
            break;
        case OBJ_UPVALUE:
            printf("upvalue");
            fprintf(stderr, "unreachable");
            exit(1);
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        default: fprintf(stderr, "printObject: unreachable"); exit(1);
    }
}
