CC=g++

SRCDIR=src/
BLDDIR=build/
BINDIR=bin/

CFLAGS=-I. -std=c++17 -lstdc++fs

all: $(BLDDIR)main.o
	$(CC) $(CFLAGS) -o $(BINDIR)puzzle $(BLDDIR)main.o

$(BLDDIR)%.o: $(SRCDIR)%.cpp $(SRCDIR)common_types.hpp $(SRCDIR)blackMagicIterator.hpp
	$(CC) -c -o $@ $< $(CFLAGS)
