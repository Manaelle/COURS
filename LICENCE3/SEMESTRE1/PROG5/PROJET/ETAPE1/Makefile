CC=gcc
CFLAGS= -Wall -g

all : readelf

readelf : $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
	
