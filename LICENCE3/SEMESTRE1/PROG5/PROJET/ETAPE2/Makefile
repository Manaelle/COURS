CC=gcc
CFLAGS= -Wall -g

all : myreadelf

myreadelf : $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
	
