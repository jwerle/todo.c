
SOPHIA_SRC = $(wildcard sophia/db/*.c)
SRC = $(SOPHIA_SRC) $(wildcard deps/*.c) $(wildcard src/*.c)
EXEC = todo todo-new todo-ls todo-completion todo-init
CFLAGS = -std=c99 -Wall -Isophia/db -Ideps
TEST_BIN = todo-test
PREFIX = /usr/local


all: clean test build

test:
	$(CC) $(SRC) tests/main.c $(CFLAGS) -o todo-test
	./todo-test

build:
	@for bin in $(EXEC); do\
		$(CC) $(SRC) $$bin.c $(CFLAGS) -o $$bin;\
	done;

clean:
	rm -f $(TEST_BIN)
	rm -f $(EXEC)
	rm -rf test-db

install:
	@for bin in $(EXEC); do\
		install $$bin $(PREFIX)/bin \\n;\
	done;
	cp todo-completions.sh /etc/todo-completions

uninstall:
	for bin in $(EXEC); do\
		rm -f $(PREFIX)/bin/$$bin;\
	done;
	rm -f /etc/todo-completions


.PHONY: all test build clean $(EXEC)
