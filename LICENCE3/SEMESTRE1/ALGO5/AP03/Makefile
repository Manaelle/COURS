
CFLAGS= -Wall -Werror

test_fap: test_fap.o fap.o
	gcc $(CFLAGS) -o test_fap test_fap.o fap.o

simul_fap : simul_fap.o fap.o
	gcc $(CFLAGS) -o simul_fap simul_fap.o fap.o

fap.o : fap.c fap.h
	gcc $(CFLAGS) -c fap.c

simul_fap.o : simul_fap.c fap.h
	gcc $(CFLAGS) -c simul_fap.c

test_fap.o : test_fap.c fap.h
	gcc $(CFLAGS) -c test_fap.c

clean:
	rm -f simul_fap test_fap *.o
