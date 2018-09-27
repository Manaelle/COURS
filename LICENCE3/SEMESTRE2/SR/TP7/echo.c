/*
 * echo - read and echo text lines until client closes connection
 */
#include <string.h>
#include "csapp.h"

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    pid_t pid;
		
		//lecture du nom de fichier 
    Rio_readinitb(&rio, connfd);
    if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("Demande de lecture du fichier %s", buf);
        Rio_writen(connfd, buf, n);
    }
    
    //A CORRIGER : mettre buf à la place de texte.txt => il faut suppr les caractères de fin...
    char *arguments[] = { "cat", "texte.txt" , NULL };
    
    //Le fils crée un sous fils qui va lire le contenu du fichier
    if ((pid = fork()) == -1) {
							perror("ERROR FORK");
							exit(1);
							}
							
		// lecture du contenu du fichier par le sous-fils					
		if (pid == 0){
		
			if (execvp("cat", arguments ) == -1) {
						perror("ERROR EXEC");
						exit(1);
				}
		}
		else {
			waitpid( pid, NULL, 0);
		}				
    
    
}

