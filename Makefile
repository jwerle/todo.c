

SOPHIA_SRC = $(wildcard sophia/db/*.c)
SRC = $(SOPHIA_SRC) $(filter-out src/main.c, $(wildcard src/*.c))
CFLAGS = -std=c99 -Wall -Isophia/db
BIN = todo
TEST_BIN = todo-test
PREFIX = /usr/local


all: clean test build

test:
	$(CC) $(SRC) tests/main.c $(CFLAGS) -o todo-test
	./todo-test

build:
	$(CC) $(SRC) src/main.c $(CFLAGS) -o todo

clean:
	rm -f $(TEST_BIN)
	rm -f $(BIN)
	rm -rf test-db

install:
	install $(BIN) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/($BIN)


.PHONY: all test build clean
