/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
//Le maitre écoute TOUJOURS ses clients sur le PORT_CLIENT, et envoie ses données aux esclaves sur les PORT_MAITRE définie dans la struct esclave (=port_esclave)

typedef struct esclave {
	int port_esclave, port_client;
	char *host;
	int slavefd;
	
} esclave;

//----------------------------------------------------
void handler(){
	
	  
    printf("\nArrêt du serveur maitre.\n");
	printf("**************************************\n");

    exit(0);
		
}

//----------------------------------------------------
int main(int argc, char **argv){
	
    int listenfd, connfd,indice_courant;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
	rio_t rio;
	size_t n;
	size_t nw;
	char buf[MAXLINE];
	int NBESCLAVES = atoi(argv[1]);
	int PORT_CLIENT = atoi(argv[2]); //à priori 2121
	int PORT_MAITRE = atoi(argv[3]); //à priori 2000
	int port_libre = PORT_CLIENT+1;
	esclave list_slave[NBESCLAVES];
	
	
	
	signal(SIGINT, handler);
	
    if (argc < 1) {
        fprintf(stderr, "ERREUR : <nb esclaves> <port client> <host esclave 1> <host esclave 2> etc...\n");
        exit(0);
    }
	
	printf("**************************************\n");
	printf("Démarrage du serveur (Maitre) sur le port %d. \n", PORT_CLIENT);
	clientlen = (socklen_t)sizeof(clientaddr);
	listenfd = Open_listenfd(PORT_CLIENT);	
	
	
	
    //Tableau des ports/host des esclaves pour la création de sockets avec tous les esclaves (en tant que client)
    for(indice_courant=0; indice_courant<NBESCLAVES; indice_courant++){
		list_slave[indice_courant].port_client = PORT_CLIENT+indice_courant+1; //port sur lequel l'esclave communique avec le client
		list_slave[indice_courant].port_esclave = PORT_MAITRE+indice_courant; //port sur lequel l'esclave communique avec le maitre
		list_slave[indice_courant].host = argv[indice_courant+4];
	}
	
	//Le maitre "tourne" autour des esclaves jusqu'à arrêt de celui-ci par ctrl-C, grace à indice_courant
	indice_courant = 0;
	
	while(1){
		printf("**************************************\n");
		//CONNEXION CLIENT (en tant que serveur)
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);
		Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);

		printf("Nouveau client : %s (%s)\n", client_hostname, client_ip_string);
		
		//Envoi d'un port libre au client
		list_slave[indice_courant].port_client = port_libre;
		sprintf(buf,"%d\n", list_slave[indice_courant].port_client);
		
		nw = rio_writen(connfd, buf, MAXLINE);
		if  ( nw != -1 ){
			printf("Port %d envoyé au client.\n", port_libre);
		}
		else {
			printf("ERREUR : Echec de l'envoi du port. \n");
		}		
				
		close(connfd);
		printf("Attente du client suivant. \n");
		
		printf("**************************************\n");
		
		//CONNEXION ESCLAVE (en tant que client)
		printf("Tentative de connexion avec le serveur esclave %s (port : %d).\n", list_slave[indice_courant].host, list_slave[indice_courant].port_esclave);	
		list_slave[indice_courant].slavefd = Open_clientfd(list_slave[indice_courant].host, list_slave[indice_courant].port_esclave); 
		
		//le maitre envoie le hostname du client + son port à l'esclave sélectionné
		sprintf(buf,"%s %d\n", client_hostname, port_libre);
		nw = rio_writen(list_slave[indice_courant].slavefd, buf, MAXLINE);
		if  ( nw != -1 ){
			printf("Hostname + port envoyé au port %d \n", list_slave[indice_courant].port_esclave);
		}
		else {
			printf("ERREUR : Echec de l'envoi. \n");
		}
		
		close(list_slave[indice_courant].slavefd);
		
		//on passe à l'esclave suivant ici
		port_libre++;
		indice_courant = (indice_courant+1)%NBESCLAVES;
	}
	return 0;
}

