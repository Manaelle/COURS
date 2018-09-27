#$(wildcard *.c) liste tous les .c
#$(patsubst %.c, %.o, fichier.c ) transforme fichier.c en fichier.o
CC=gcc
CFLAGS=-Wall -Werror

all: essai_fap

essai_fap: $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
essai: 
	gcc essai.c -o essai
	
somme: 
	gcc somme.c -o somme
	
clean:
	rm -rf *.o
	
rinit:
	rm somme *.o
