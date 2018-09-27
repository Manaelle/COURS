#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    int tubeA[2];
    int tubeB[2];
    char message[100] = "Hello World";

		//CREATION DES TUBES A ET B
    if (pipe(tubeA) == -1) {
        fprintf(stderr, "Erreur de création du tube A\n");
        exit(1);
    }
    
    if (pipe(tubeB) == -1) {
        fprintf(stderr, "Erreur de création du tube B\n");
        exit(1);
    }
    
    //CREATION DU FILS
    pid = fork();
    
    //TRANSMISSION DU MESSAGE
    switch (pid) {
      case -1:
        fprintf(stderr, "Erreur de fork\n");
        exit(2);
      
      case 0: //fils
        close(tubeA[1]);
        close(tubeB[0]);
        
        char val[100];
        
        read(tubeA[0], &val, sizeof(val));
        printf("Je suis le fils, j'ai lu le message %s dans le tube\n", val);
        write(tubeB[1], &val, sizeof(val));
        printf("Je suis le fils, j'envoie le message %s à mon père\n", val);
        
        close(tubeA[0]);
        close(tubeB[1]);
        break;
        
      default: //pere
        close(tubeA[0]);
        close(tubeB[1]);
        
        char val2[100];
        
        printf("Je suis le père, j'envoie le message %s à mon fils\n", message);
        write(tubeA[1], &message, sizeof(message));
        read(tubeB[0], &val2, sizeof(val2));
        printf("Je suis le père, j'ai lu le message %s dans le tube\n", val2);
        
        close(tubeA[1]);
        close(tubeB[0]);
    }
    return 0;
}
