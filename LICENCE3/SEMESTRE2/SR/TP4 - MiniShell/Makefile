CC=gcc
CFLAGS=-Wall -g -Werror

all: shell

chrono : chrono.o
	$(CC) -o chrono chrono.o $(CFLAGS)
	
chrono.o : chrono.c
	$(CC) -c chrono.c $(CFLAGS)
	
letter : letter.o
	$(CC) -o letter letter.o $(CFLAGS)
	
letter.o : letter.c
	$(CC) -c letter.c $(CFLAGS)
	
shell : shell.o readcmd.o fap.o
	$(CC) -o shell shell.o readcmd.o fap.o $(CFLAGS)
	
readcmd.o : readcmd.c readcmd.h fap.h
	$(CC) -c readcmd.c $(CFLAGS)
	
fap.o : fap.c fap.h
	$(CC) -c fap.c $(CFLAGS)
	
shell.o : shell.c readcmd.h fap.h
	$(CC) -c shell.c $(CFLAGS)
	
	

clean:
	rm -f *.o shell
