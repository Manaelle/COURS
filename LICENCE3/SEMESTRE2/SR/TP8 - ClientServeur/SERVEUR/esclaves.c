/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NPROC 4

int echo(int connfd);

//----------------------------------------------------
void handler(){
	
	int status, i;
	pid_t pid;
	  
    printf("\nArrêt du serveur.\n");
	printf("**************************************\n");
    kill(0, SIGINT);

    exit(0);
		
}

//----------------------------------------------------
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, clientfd, port_maitre, client_port;
    socklen_t masterlen;
    struct sockaddr_in masteraddr;
    char master_ip_string[INET_ADDRSTRLEN];
    char master_hostname[MAX_NAME_LEN], client_hostname[MAX_NAME_LEN];
    pid_t pid, pid_pere;
    int status;
    int i, echo_err;
	rio_t rio;
	size_t n, nw;
    pid_t proc[NPROC];
	char buf[MAXLINE];

    if (argc < 1) {
        fprintf(stderr, "ERREUR : usage: %s auto port 2121\n", argv[0]);
        exit(0);
    }
	
	printf("**************************************\n");
	printf("Démarrage du serveur (esclave). \n");

	pid_pere = getpid();
    port_maitre = atoi(argv[1]);
	printf("Attente du maitre sur le port %d...\n", port_maitre);
	masterlen = (socklen_t)sizeof(masteraddr);
	listenfd = Open_listenfd(port_maitre); 
    
    
	
	//AJOUTE : Gestion des clients par les NPROC fils
    for(i=0; i<NPROC; i++){
    	if (getpid() == pid_pere){
					
					if ((pid = fork()) == -1) {
							perror("ERROR FORK");
							exit(1);
					}
					else {
						proc[i] = pid;
					}
				} 				
	}
		
		// FILS	
		if (pid == 0){
				while (1) {
					
					//connexion avec le maitre en tant que serveur
				    connfd = Accept(listenfd, (SA *)&masteraddr, &masterlen);
				    Getnameinfo((SA *) &masteraddr, masterlen, master_hostname, MAX_NAME_LEN, 0, 0, 0);
					Inet_ntop(AF_INET, &masteraddr.sin_addr, master_ip_string, INET_ADDRSTRLEN);
					printf("Connexion avec le maitre : %s (%s)\n", master_hostname, master_ip_string);
					
					//réception du hostname + du port du client
					Rio_readinitb(&rio, connfd);
					if ( (n = rio_readlineb(&rio, buf, MAXLINE)) != 0){	
						buf[n-1] ='\0';
						printf("Host + port du client reçu : %s \n", buf);
					}
					else {
						fprintf(stderr, "%s\n", strerror(errno));
						return -1;
					}
					
					//mini split_line juste pour séparer le hostname et port
					char *word = strtok(buf, " ");
					strcpy(client_hostname, word);
					word = strtok(NULL, " ");
					char buf2[MAXLINE];
					strcpy(buf2, word);
					client_port = atoi(buf2);
					
					close(connfd);
					
					printf("**************************************\n");
					//connexion avec le vrai client en tant que client
					printf("Tentative de connexion avec le client %s (port : %d)\n", client_hostname, client_port);
					if ((clientfd = Open_clientfd(client_hostname, client_port)) < 0){
						printf("Refus de connexion. \n" );	
					}
					else {
					//ECHO : coeur du traitement
				    echo_err = echo(clientfd);
				    Close(clientfd);
						if (echo_err == 0){
							printf("Terminé : Le client s'est déconnecté. \n");
						}
						else{
							printf("Déconnection non-prévu du client. \n" );
						}
					}
				}
		}
			
			//PERE
			else {				
					signal(SIGINT, handler);
					while(1){
					
					} 
			}
}

