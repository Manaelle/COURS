#include <stdio.h>
#include "csapp.h"
#define NMAX 10

int main(int argc, char *argv[]){
	
	pid_t pid_actual;
	pid_t pid[NMAX]; // On va stocker tous nos pid
	int i, p = 1;
	
	pid[0] = getpid(); //Pid du père ultime !
	printf("PERE ULTIME =>    pid[%d] : %d \n", 0, pid[0]);
	
	for(i = 1; i <= atoi(argv[1]) ; i++)
	{
			//Rappel : Fork renvoie le pid du fils pour le processus père, et 0 si c'est un fils.
			pid_actual = Fork();
			if (getpid() == pid[0]) 
			{
				printf("Noeud n°%d :\n", i);
				pid[p] = pid_actual; // On ajoute les fils au fur et à mesure.
				p++;
			}
			if (pid_actual == 0) 
			{
				//Ce que fait le FILS
				printf("	PID du père : %d \n", getppid());
				printf("	PID : %d \n \n", getpid());
				pid[p] = getpid(); // On ajoute les fils au fur et à mesure.
				p++;
			}
			
			if (pid_actual != 0)
			{	//Ce que fait le PERE
				printf("	PID noeud : %d \n \n", getpid());
			}
			
	
	}
	
	printf("END =>    pid_actual : %d \n", getpid());
	
	if (getpid() == pid[0]) 
			{
				printf("Tableau PID :\n");
				for(i=0; i<NMAX; i++)
				{
					printf("	pid[%d] : %d \n", i, pid[i]);
				}
			}
			
	
	return 0;
}
