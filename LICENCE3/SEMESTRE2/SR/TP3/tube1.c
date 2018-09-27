#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    int tube[2];
    long entier;
    char c ='a';

    if (pipe(tube) == -1) {
        fprintf(stderr, "Erreur de création du tube\n");
        exit(1);
    }
    pid = fork();
  
    while(c !='0') {
		  switch (pid) {
		    case -1:
		      fprintf(stderr, "Erreur de fork\n");
		      exit(2);
		    case 0:
		    	//Fils
		      close(tube[1]);
		      	read(tube[0], &c, sizeof(c));
		      	printf("Fils : %c \n", c);
		      break;
		    default:
		    	//père
		      close(tube[0]);
		      printf("Père : \n");
		      scanf("%c", &c);
		      write(tube[1], &c, sizeof(c));
		  }
    }
    return 0;
}

