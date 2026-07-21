#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)    (AS_OBJ(value)->type)

#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)   isObjType(value, OBJ_NATIVE)
#define IS_CLOSURE(value)  isObjType(value, OBJ_CLOSURE)
#define IS_STRING(value)   isObjType(value, OBJ_STRING)

#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_NATIVE(value)   ((ObjNative *)AS_OBJ(value))
#define AS_CLOSURE(value)  ((ObjClosure *)AS_OBJ(value))
#define AS_STRING(value)   ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_CLOSURE,
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

struct ObjFunction {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString *name;
};

typedef Value (*NativeFn)(int argCount, Value *args);

struct ObjNative {
    Obj obj;
    NativeFn function;
    int arity;
};

struct ObjClosure {
    Obj obj;
    ObjFunction *function;
};

struct ObjString {
    Obj obj;
    int length;
    uint32_t hash;
    char chars[];
};

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjFunction *newFunction();

ObjNative *newNative(NativeFn function, int arity);

ObjClosure *newClosure(ObjFunction *function);

ObjString *takeString(char *chars, int length);
ObjString *copyString(const char *chars, int length);

void printObject(Value value);

#endif
