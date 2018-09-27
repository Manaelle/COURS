CC=gcc
CFLAGS= -lpthread

all: echoclient echoserveri echo

csapp.o : csapp.c csapp.h
	$(CC) -c csapp.c $(CFLAGS)

echoclient : echoclient.o csapp.o echo.o
	$(CC) -o echoclient echo.o csapp.o echoclient.o $(CFLAGS)
	
echoclient.o : echoclient.c csapp.h
	$(CC) -c echoclient.c $(CFLAGS)
	
echoserveri : echoserveri.o csapp.o echo.o
	$(CC) -o echoserveri echo.o csapp.o echoserveri.o $(CFLAGS)
	
echoserveri.o : echoserveri.c csapp.h
	$(CC) -c echoserveri.c $(CFLAGS)

echo : echo.o csapp.o
	$(CC) -o echo csapp.o echo.o $(CFLAGS)
	
echo.o : echo.c csapp.h
	$(CC) -c echo.c $(CFLAGS)	
	

	
clean:
	rm -rf *.o
	
