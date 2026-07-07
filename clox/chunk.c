#include "chunk.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines,
            oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeConstant(Chunk *chunk, Value value, int line) {
    int i = addConstant(chunk, value);
    if (i < (1 << 8)) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, i, line);
    }
    else if (i < (1 << 24)) {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, (i >>  0) & 0xFF, line);
        writeChunk(chunk, (i >>  8) & 0xFF, line);
        writeChunk(chunk, (i >> 16) & 0xFF, line);
    }
    else {
        printf("Maximum constants in chunk %p exceeded\n", (void*)chunk);
        exit(1);
    }
}
