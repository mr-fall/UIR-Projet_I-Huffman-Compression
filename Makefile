# Makefile
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2
OBJS = main.o huffman.o minheap.o

all: huff

huff: $(OBJS)
	$(CC) $(CFLAGS) -o huff $(OBJS)

main.o: main.c huffman.h
	$(CC) $(CFLAGS) -c main.c

huffman.o: huffman.c huffman.h minheap.h
	$(CC) $(CFLAGS) -c huffman.c

minheap.o: minheap.c minheap.h huffman.h
	$(CC) $(CFLAGS) -c minheap.c

clean:
	rm -f *.o huff