#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    pid_t pid;
    int tube[2];
    
    FILE *fp;
   	char buffer[100];


		//CREATION DU TUBE
    if (pipe(tube) == -1) {
        fprintf(stderr, "Erreur de création du tube\n");
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
        close(tube[1]);
        
        char val[100];
        
        read(tube[0], &val, sizeof(val));
        printf("Reception du fils :\n");
        printf("%s", val);
        
        close(tube[0]);
        break;
        
      default: //pere
        close(tube[0]);
        
        //LECTURE DU FICHIER
        fp = fopen("test.txt", "r+");
        fread(buffer, 100, 1, fp);
        
        printf("Envoie du père\n");
        write(tube[1], &buffer, sizeof(buffer));
        
        close(tube[1]);
        fclose(fp);
    }
    return 0;
}
