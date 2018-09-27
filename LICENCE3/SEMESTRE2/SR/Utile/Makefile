
CFLAGS= -Wall -Werror

all : test_fap
	
test_fap: test_fap.o fap.o
	gcc $(CFLAGS) -o test_fap test_fap.o fap.o 

fap.o : fap.c fap.h
	gcc $(CFLAGS) -c fap.c


clean:
	rm -f *.o