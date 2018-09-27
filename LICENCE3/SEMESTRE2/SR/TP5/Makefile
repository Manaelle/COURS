CC=gcc
CFLAGS= -lpthread

all: shm_server shm_client

shm_server : shm_server.o 
	$(CC) -o shm_server shm_server.o $(CFLAGS)
	
shm_server.o : shm_server.c
	$(CC) -c shm_server.c $(CFLAGS)
	
shm_client : shm_client.o 
	$(CC) -o shm_client shm_client.o $(CFLAGS)
	
shm_client.o : shm_client.c
	$(CC) -c shm_client.c $(CFLAGS)

mask : mask.o 
	$(CC) -o mask mask.o $(CFLAGS)
	
mask.o : mask.c
	$(CC) -c mask.c $(CFLAGS)	
	
csapp.o : csapp.c csapp.h
	$(CC) -c csapp.c $(CFLAGS)
	
tell_wait.o : tell_wait.c tell_wait.h
	$(CC) -c tell_wait.c $(CFLAGS)
	
clean:
	rm -rf *.o
	
