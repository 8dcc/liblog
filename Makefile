
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic
LDFLAGS=

BIN=log_test.out

#-------------------------------------------------------------------------------

.PHONY: clean all

all: $(BIN)

clean:
	rm -f $(BIN)

#-------------------------------------------------------------------------------

$(BIN): src/main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
