
CFLAGS= -Wall -Werror

test_arbre: test_arbre.o fap.o arbre_recherche.o
	gcc $(CFLAGS) -o test_arbre test_arbre.o fap.o arbre_recherche.o
	
test_fap: test_fap.o fap.o
	gcc $(CFLAGS) -o test_fap test_fap.o fap.o 

fap.o : fap.c fap.h
	gcc $(CFLAGS) -c fap.c

arbre_recherche.o : arbre_recherche.c arbre_recherche.h
	gcc $(CFLAGS) -c arbre_recherche.c


clean:
	rm -f test_arbre *.o