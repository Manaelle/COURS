#$(wildcard *.c) liste tous les .c
#$(patsubst %.c, %.o, fichier.c ) transforme fichier.c en fichier.o
CC=gcc
CFLAGS=-Wall -Werror

all: essai_pile

essai_pile: $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
	
rinit:
	rm essai_pile *.o
