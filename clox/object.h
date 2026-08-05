#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "chunk.h"
#include "table.h"
#include "value.h"

#define OBJ_TYPE(value)    (AS_OBJ(value)->type)

#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)   isObjType(value, OBJ_NATIVE)
#define IS_CLOSURE(value)  isObjType(value, OBJ_CLOSURE)
#define IS_CLASS(value)    isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)
#define IS_STRING(value)   isObjType(value, OBJ_STRING)

#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_NATIVE(value)   ((ObjNative   *)AS_OBJ(value))
#define AS_CLOSURE(value)  ((ObjClosure  *)AS_OBJ(value))
#define AS_CLASS(value)    ((ObjClass    *)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance *)AS_OBJ(value))
#define AS_STRING(value)   ((ObjString   *)AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString  *)AS_OBJ(value))->chars)

typedef enum {
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_CLOSURE,
    OBJ_CLASS,
    OBJ_INSTANCE,
    OBJ_UPVALUE,
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    struct Obj *next;
};

const char *ObjTypeName(ObjType type);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

struct ObjFunction {
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString *name;
};

ObjFunction *newFunction();

typedef Value (*NativeFn)(int argCount, Value *args);

struct ObjNative {
    Obj obj;
    NativeFn function;
    int arity;
};

ObjNative *newNative(NativeFn function, int arity);

struct ObjClosure {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
};

ObjClosure *newClosure(ObjFunction *function);

struct ObjClass {
    Obj obj;
    ObjString *name;

};

ObjClass *newClass(ObjString *name);

struct ObjInstance {
    Obj obj;
    ObjClass *klass;
    Table fields;
};

ObjInstance *newInstance(ObjClass *klass);

struct ObjUpvalue {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
};

ObjUpvalue *newUpvalue(Value *slot);

struct ObjString {
    Obj obj;
    int length;
    uint32_t hash;
    char chars[];
};

ObjString *takeString(char *chars, int length);
ObjString *copyString(const char *chars, int length);

void printObject(Value value);

#endif
