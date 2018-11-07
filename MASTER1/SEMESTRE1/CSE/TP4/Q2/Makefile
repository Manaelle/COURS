CC=gcc
CFLAGS=-Wall -Werror -g -lm
LDLIBS=-lpthread

all:	test_lecteurs_redacteurs

test_lecteurs_redacteurs:	test_lecteurs_redacteurs.o lecteur_redacteur.o

lecteur_redacteur.o:	lecteur_redacteur.c
	$(CC) $(CFLAGS) -c lecteur_redacteur.c

test_lecteurs_redacteurs.o:
	$(CC) $(CFLAGS) -c test_lecteurs_redacteurs.c

clean:
	rm test_lecteurs_redacteurs *.o
