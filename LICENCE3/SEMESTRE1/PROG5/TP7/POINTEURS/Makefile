CC=gcc
CFLAGS=-Wall -Werror

all: essai_fap

essai_fap : $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -g -o $@ $^
	
%.o : %.c
	$(CC) -g -c $^
	
clean:
	rm -rf *.o
	
