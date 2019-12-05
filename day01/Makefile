CC=clang++

SRCDIR=src/
BLDDIR=build/
BINDIR=bin/

CFLAGS=-I. -std=c++17

all: $(BLDDIR)main.o
	$(CC) $(CFLAGS) -o $(BINDIR)puzzle $(BLDDIR)main.o

$(BLDDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)