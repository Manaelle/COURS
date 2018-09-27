#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include "TP1_UTILE.h"

#define BUFFSIZE 100
#define ECRITURE 1
#define LECTURE 0

//-------------------------------------
int getID(pid_t pid, int n, processus P[]){
	int i;
	
	for (i=0; i<n-1; i++) {
		if(P[i].pid == pid){
			return P[i].ID;
		}
	}
	return -1;
}
//-------------------------------------
void creer_fils(int n, processus P[], int tubes[][2]){

	int status;
	int i;
	pid_t pid;
	
	//INITIALISATION
	P[0].pid = getpid(); //processus patriarche
	P[0].ID = 0;
	
	//CREATION DES TUBES
	for (i=0; i<n-1; i++) {
		if (pipe(tubes[i]) == -1) {
    		printf("ERREUR : Echec de la création du tube %d. \n", i);
			exit(0);
    	}
	}
	
	//CREATION DES PROCESSUS
	for (i=0; i<n-1; i++) {
		
		if(getpid() == P[i].pid){
			pid=fork();
		
			switch(pid){
				case -1 : 
					printf("ERREUR : Echec de la création du processus %d. \n", i);
					break;

				case 0: //FILS
					P[i+1].pid=getpid();
					P[i+1].ID = i+1;
					printf("FILS n° %d \n",P[i+1].pid);
					
					close(tubes[i][ECRITURE]);
					break;
						
				default : //PERE
					close(tubes[i][LECTURE]);
					
					waitpid(P[i+1].pid, &status, WNOHANG);
					break;
			
			}
		}
	}
	
	//TRANSMISSION DE MESSAGES A PARTIR DU PROCESSUS 0 (patriarche)
	if(getpid() == P[0].pid){
			char val[BUFFSIZE] = "Ok ok ! :D ";
			write(tubes[0][ECRITURE], &val, sizeof(val));
					
			close(tubes[0][ECRITURE]);
	}
	else{
			char buffer[BUFFSIZE];
			read(tubes[getID(getpid(), n, P)][LECTURE], &buffer, sizeof(buffer));
			printf("Fils : Je reçois '%s' de mon père n°%d \n", buffer, getppid());
			
			close(tubes[getID(getpid(), n, P)][LECTURE]);
	}
	
	//VERIFICATION DE DONNEES
	if(getpid() == P[0].pid){
		
		printf("\nPROCESSUS ACTUEL : \n%d \n\n", getpid());
		printf("PROCESSUS : \n%d \n", P[0].pid);
		
		for(i=0; i<n-1; i++){
			printf("%d \n", P[i].pid);
		}
	}
	
}
//-------------------------------------
int main(int argc, char *argv[]) {
	
	int n = atoi(argv[1]);
	processus P[n];
	int tubes[n-1][2];
	
	creer_fils(n, P, tubes);

	return 0;
}
