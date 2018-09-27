#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void creer_fils(int n, pid_t pid, int *status, int** tubes, int valeur){
    
    if(n ==	0){
    	return;
    	
    }
    else{
    	
    	int val;
    	
    	pid=fork();
    
		  switch(pid){
		  		case -1 : printf("ERREUR");
		  							break;
		  							
		  		case 0: //FILS
		  						close(tubes[n][1]);
		  						read(tubes[n][0], &val, sizeof(val));
		  						printf("Fils : Je reçois %d de mon père \n", val);
		  						creer_fils(n-1, pid, status, tubes, val);
		  						close(tubes[n][0]);
		  						break;
		  		
		  		default : 
		  							val = valeur;
		  							close(tubes[n][0]);
		  							printf("Père : J'envoie %d à mon fils \n", val);
		  							write(tubes[n][1], &val, sizeof(val));
		  							close(tubes[n][1]);
		  							waitpid(pid, status, 0);
		  							break;
		  						
		  	}
    }
    
}
//--------------------------------------
int main(int argc, char *argv[]) {
    
    int n = atoi(argv[1]);
    pid_t pid;
    int *status;
    
    if(n<0){
			printf("ERREUR");
			return 0;
		}
		
		printf("Fils à créer : %d \n", n);
		
		int tubes[n][2];
		int k;
		for (k = 0; k<n; k++) {
			if (pipe(tubes[k]) == -1) {
    		printf("ERROR");
    	}
		}
		
		srandom(pid);
		
		creer_fils(n, pid, status, tubes, random());
    
    return 0;
}

//--------------------------------------

