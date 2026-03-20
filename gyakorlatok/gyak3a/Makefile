CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99
LIBS_WIN = -lmingw32 -lSDL2main -lSDL2 -lm
LIBS_LINUX = -lSDL2 -lm

SOURCES = main.c line.c
EXECUTABLE = program.exe

all: windows

windows:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS_WIN) -o $(EXECUTABLE)

linux:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS_LINUX) -o program

clean:
	del $(EXECUTABLE)

run: all
	$(EXECUTABLE)

.PHONY: all windows linux clean run
