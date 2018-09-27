CC=gcc
CFLAGS= -lpthread

all: TP1_EXO6

TP1_EXO6 : TP1_EXO6.o
	$(CC) -o TP1_EXO6 TP1_EXO6.o $(CFLAGS)
	
TP1_EXO6.o : TP1_EXO6.c
	$(CC) -c TP1_EXO6.c $(CFLAGS)

	
TP1_UTILE : TP1_UTILE.o
	$(CC) -o TP1_UTILE TP1_UTILE.o $(CFLAGS)
	
TP1_UTILE.o : TP1_UTILE.c TP1_UTILE.h
	$(CC) -c TP1_UTILE.c $(CFLAGS)
	
clean:
	rm -rf *.o
	
