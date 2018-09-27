#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "csapp.h"
#define SIZE 10

main(){
	
	pid_t pid;
	int t, c1, c2;
	int shmid;
  key_t key;
	
	
	// Création d'un c1 et c2 partagé
	if ((partage = shmget(key, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
	
	if((pid = fork()) < 0)
	{
		perror("ERREUR FORK");
		exit(1);
	}
	
	/*FILS*/
	else if (pid == 0)
	{
		c1 = 1;
		t = 2;
		while ((c2 == 1) && (t == 2));
		
		//Section critique !
		if (execvp("./shm_client", "") == -1) {
						perror("ERROR EXEC");
						exit(1);
		}
		
		c1 = 0;
	}
	
	/*PERE*/
	else
	{
		c2 = 1;
		t = 1;
		while ((c1 == 1) && (t == 1));
		
		if (execvp("./shm_server", "") == -1) {
						perror("ERROR EXEC");
						exit(1);
		}
		c2 = 0;
	}

	return 0;
}
