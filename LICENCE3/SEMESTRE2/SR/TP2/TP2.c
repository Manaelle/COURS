#include <stdio.h>
#include <unistd.h>
#include "csapp.h"
#define NMAX 10

void handler(int sig) /* SIGINT handler */
{
	printf("Caught SIGINT\n");
	exit(0);

}

//----------------------------------------
//Question 1
/*int main(int argc, char *argv[])
{
	int temps;
	
	Signal(SIGINT, handler); //Envoie un signal après Ctrl+C
	temps = sleep(atoi(argv[1]));
	printf("Temps d'attente : %d \n", atoi(argv[1]) - temps);
	exit(0);
}*/

//----------------------------------------
//Question 2 : SIGKILL et SIGSTOP provoquent un signal error
int main(int argc, char *argv[])
{
	
	Signal(SIGINT, handler); //Envoie un signal après Ctrl+C
	pause();
	exit(0);
}
