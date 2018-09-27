/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NPROC 3

void echo(int connfd);


//----------------------------------------------------
//AJOUTEE : Lorsque le père recoit SIGCHLD (=il apprend la mort d'un fils), il tue le processus zombie (grace au wait et à l'option WHOHANG)
void handler(){
	
	int status, i;
	pid_t pid;
	
	// Le while est là en cas où un processus termine pendant l'exécution du handler
	/*while ( (pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		printf("Arrêt du client \n");
    }*/
    
    printf("\nArrêt du serveur.\n");
    kill(0, SIGKILL);

    exit(0);
		
}

//----------------------------------------------------
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    pid_t pid, pid_pere;
    int status;
    int i;
    pid_t Jobs[NPROC];
    
    if (argc != 2) {
        fprintf(stderr, "ERREUR : usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    
    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(port);
    
    pid_pere = getpid();
    
    //AJOUTE : Gestion des clients par les NPROC fils
    for(i=0; i<NPROC; i++){
    	if (getpid() == pid_pere){
					
					if ((pid = fork()) == -1) {
							perror("ERROR FORK");
							exit(1);
							}
					else {
						Jobs[i] = pid;
							}
				} 				
			}
		
		// FILS	
		if (pid == 0){
		
				while (1) {
							    
				    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

				    /* determine the name of the client */
				    Getnameinfo((SA *) &clientaddr, clientlen,
				                client_hostname, MAX_NAME_LEN, 0, 0, 0);
				    
				    /* determine the textual representation of the client's IP address */
				    Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
				              INET_ADDRSTRLEN);
				    
				    printf("Nouveau client : %s (%s)\n", client_hostname,
				           client_ip_string);

				    echo(connfd);
				    Close(connfd);
				    printf("Terminé : Le client a été arrêté. \n");
				}
				
			}
			
			//PERE
			else {				
					signal(SIGINT, handler);
					while(1){} 
			}
}

