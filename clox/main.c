#define _POSIX_C_SOURCE 199309L

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *readFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file \"%s\"\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\"\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\"\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void repl() {
    char line[1024];
    for (;;) {
        printf("clox> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static void runFile(const char *path) {
    char *source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char **argv) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);

    bool interactive = false;
    int o;
    while ((o = getopt(argc, argv, "i")) != -1) {
        switch (o) {
            case 'i':
                interactive = true;
                break;
            default:
                break;
        }
    }
    argc -= optind;
    argv += optind;

    initVM();

    if (argc == 0) {
        repl();
    }
    else if (argc == 1) {
        runFile(argv[0]);
        if (interactive) {
            repl();
        }
    }
    else {
        fprintf(stderr, "usage: clox [progfile]\n");
    }

    freeVM();
    return 0;
}
