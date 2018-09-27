CC=gcc
CFLAGS=-Wall -Werror

all: bitabit

bitabit: $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
	
