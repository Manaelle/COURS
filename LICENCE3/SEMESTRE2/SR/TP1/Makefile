CC=gcc
CFLAGS= -lpthread

all: test_pid

test_pid : test_pid.o csapp.o 
	$(CC) -o test_pid test_pid.o csapp.o $(CFLAGS)
	
test_pid.o : test_pid.c csapp.h
	$(CC) -c test_pid.c $(CFLAGS)
	
csapp.o : csapp.c csapp.h
	$(CC) -c csapp.c $(CFLAGS)
	
clean:
	rm -rf *.o
	
