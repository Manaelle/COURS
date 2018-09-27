all: parenthesage

test_pile: test_pile.o pile.o
	gcc -o test_pile test_pile.o pile.o

parenthesage: parenthesage.o pile.o generateur_test.o
	gcc -o parenthesage parenthesage.o pile.o generateur_test.o

parenthesage.o: parenthesage.c pile.h generateur_test.h
	gcc -o parenthesage.o -c parenthesage.c -W -Wall

generateur_test.o: generateur_test.c pile.h
	gcc -o generateur_test.o -c generateur_test.c -W -Wall

pile.o: pile.c
	gcc -o pile.o -c pile.c -W -Wall 

clean:
	rm -rf *.o
