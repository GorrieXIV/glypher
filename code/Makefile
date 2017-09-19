CC = gcc
CFLAGS = -Wall
LFLAGS = -lncurses
OBJS = glypher.o fileio.o

all: glypher

glypher: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o glyph

glypher.o: glypher.c
	$(CC) $(CFLAGS) -c glypher.c

fileio.o: fileio.c
	$(CC) $(CFLAGS) -c fileio.c
