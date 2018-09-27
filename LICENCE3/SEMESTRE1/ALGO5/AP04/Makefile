CFLAGS= -g -Wall -Werror
CC=gcc

all: huff_encode huff_decode

huff_encode : huff_encode.o arbrebin.o fap.o bfile.o bonus.o
	$(CC) $(CFLAGS) $^ -o $@

huff_decode : huff_decode.o arbrebin.o bfile.o
	$(CC) $(CFLAGS) $^ -o $@

# Dépendances obtenues avec "gcc -MM *.c"
arbrebin.o: arbrebin.c arbrebin.h
bfile.o: bfile.c bfile.h
fap.o: fap.c fap.h arbrebin.h
huff_decode.o: huff_decode.c arbrebin.h bfile.h
huff_encode.o: huff_encode.c fap.h arbrebin.h bfile.h bonus.h \
 huffman_code.h

clean:
	rm -f huff_encode huff_decode $(patsubst %.c,%.o,$(wildcard *.c))
