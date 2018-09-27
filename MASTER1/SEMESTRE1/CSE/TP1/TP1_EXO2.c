#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void creer_fils(int n, pid_t pid, int *status){
    	
    if(n ==	0){
    	return;
    	
    }
    else{
    
    	pid=fork();
    
		  switch(pid){
		  		case -1 : printf("ERREUR");
		  							break;
		  							
		  		case 0: //FILS
		  						creer_fils(n-1, pid, status);
		  						break;
		  		
		  		default : 
		  							printf("FILS CREE %d \n", pid);
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
		
		creer_fils(n, pid, status);
    
    return 0;
}

//--------------------------------------

