SRCS = main.c compiler.c chunk.c debug.c memory.c object.c scanner.c \
	   table.c value.c vm.c
OBJS = $(SRCS:.c=.o)
MAIN = clox

CFLAGS += -Wall -Werror -Wextra -std=c99 -pedantic -g3 -MMD

$(MAIN): $(OBJS)
	$(CC) -o $@ $^

run: $(MAIN)
	clear && ./$(MAIN)

.PHONY: clean
clean:
	rm -rvf $(OBJS) clox

-include $(SRCS:.c=.d)

