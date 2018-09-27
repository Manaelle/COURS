CC=gcc
CFLAGS= -lpthread

all: counterprob

counterprob : counterprob.o csapp.o 
	$(CC) -o counterprob counterprob.o csapp.o $(CFLAGS)
	
counterprob.o : counterprob.c csapp.h
	$(CC) -c counterprob.c $(CFLAGS)
	
csapp.o : csapp.c csapp.h
	$(CC) -c csapp.c $(CFLAGS)
	
clean:
	rm -rf *.o
	
