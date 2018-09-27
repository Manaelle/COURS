/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "readcmd.h"
#include "fap.h"

jobs liste; //file de pid, listant les pid des processus en tâche de fond seulement.

//----------------------------------------------------
//AJOUTEE : Lorsque le père recoit SIGCHLD (=il apprend la mort d'un fils), il tue le processus zombie (grace au wait et à l'option WHOHANG)
void handler(){
	
	int status;
	pid_t pid;
	
	// Le while est là en cas où un processus termine pendant l'exécution du handler
	while ( (pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		if (WIFSTOPPED(status)) {
			printf("Le fils %d a été stoppé ! \n", pid);
			kill(pid,SIGKILL);
		}
		else {
			printf("Le fils %d est mort ! \n", pid);
			liste.list_jobs = Extraire(liste.list_jobs, &pid);
		}
    }
		
}

//----------------------------------------------------
int main()
{
			
	liste.list_jobs = Initialiser(); //Initialise la file de pid (voir fap.c)
	liste.nbjobs = 1;
	
	
	//AJOUTEE : Si ctrl+C, le signal SIGINT est envoyé. signal(A, B) exécute B s'il reçoit le signal A.
	signal(SIGINT, testsig); // SIG_IGN ignore le signal. Ici, Ctrl+C arrête.
	signal(SIGTSTP, SIG_DFL); // Le signal Ctrl+Z stoppe
	signal(SIGCHLD, handler); // Quand un fils meurt, SIGCHLD est envoyé au père.
	
	 while (1) {
		struct cmdline *l;

		fflush(stdout);
		printf("shell> ");
		
		l = readcmd();
		
		if ( l != NULL ){
			// If input stream closed, normal termination 
			if (!l) {
				printf("exit\n");
				exit(0);
			}

			if (l->err) {
				// Syntax error, read another command 
				printf("error: %s\n", l->err);
				continue;
			}
			
			if (l->seq[0] != 0) {
				// Arret si on tape "exit" : Permet un arrêt propre (autre que ctrl+C)
				if ( strcmp( l->seq[0][0] , "exit") ==0 ) {
					break;
				}
				// Affichage des jobs si on tape "jobs"
				else if ( strcmp( l->seq[0][0] , "jobs") ==0 ){
					Afficher(liste.list_jobs);
				}
				// Arrêt du job de pid_t "pid" si on tape "stop"
				else if ( strcmp( l->seq[0][0] , "stop") ==0 ){
					stop( (pid_t)atoi(l->seq[0][1]) );
				}
				// NE FONCTIONNE PAS : Passage du processus de pid_t "pid" en premier plan si on tape "fg" (doit retirer le pid de liste)
				else if ( strcmp( l->seq[0][0] , "fg") ==0 ){
					foreground(liste, (pid_t)atoi(l->seq[0][1]) );
				}
				else {
					//AJOUTE : voir readcmd.c
					fflush(stdin);
					liste = executer(l, liste);
				}
			}
			
		}	
	} 
	
	Detruire(liste.list_jobs);
	
	return 0;
}
