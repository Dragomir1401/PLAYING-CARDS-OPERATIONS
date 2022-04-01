#Copyright Dragomir Andrei-Mihai
#compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

# define targets
TARGET=tema1

build: $(TARGET)

tema1: main.o deck_functions.o dll_deck_functions.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

pack:
	zip -FSr 312CA_DragomirAndrei_Tema1.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean