CC=g++

SRCDIR=src
BLDDIR=build
BINDIR=bin

CFLAGS=-I. -std=c++17 -lstdc++fs -g

all: $(BLDDIR)/main.o
	$(CC) $(CFLAGS) -o $(BINDIR)/puzzle $(BLDDIR)/main.o

clean:
	rm -r $(BLDDIR)/* $(BINDIR)/*

$(BLDDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
