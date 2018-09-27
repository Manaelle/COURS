/*
 * echoclient.c - An echo client
 */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include "csapp.h"
#define MAXCMD 3
#define MAX_NAME_LEN 256


//--------------------------------------
//Retourne simplement la taille (en octet) du fichier
int taille_fichier(int fichier){
	
	long int s = 0;
	struct stat info;
	int status;

	if (fichier < 0){
		NULL; //en cas d'erreur, ne rien faire 
	}
	else {
		status = fstat(fichier, &info);
		s = (int)info.st_size;
	}
	
	return s;
}
//--------------------------------------
//prend une ligne de commande et la segmente en mots dans un tableau "line".
void split_line(char cmd[MAXLINE], char line[MAXCMD][MAXLINE]){
	
	const char delim[1] = " ";
	char *word;
	int i=0;
	
	word = strtok(cmd, delim);
   
   //Pour chaque mot, le ranger dans line[i] 
   while( word != NULL && i<MAXLINE ) 
   {  
		strcpy(line[i], word);
		i++;
		word = strtok(NULL, delim);
   }
	
}

//--------------------------------------
//COMMANDE GET : prend une ligne de commande et effectue le transfert du fichier
int get(char linecmd[MAXCMD][MAXLINE], char commande[MAXLINE], int connfd){

	rio_t rio;
	int f;
	ssize_t n, nw;
	char err[MAXLINE];	
	char csizef[MAXLINE];
	int sizef, tmpsizef = 0; //sizef est la taille TOTALE du fichier à DL, tmpsizef est la taille actuel du fichier demandé

	Rio_readinitb(&rio, connfd);
	
		//CREATION/OUVERTURE DU FICHIER
		f = open( linecmd[1] , O_CREAT | O_APPEND | O_RDWR, 0666 );
		if (f < 0){
			fprintf(stderr, "ERREUR : %s\n", strerror(errno));
			exit(0);
		}

		//On envoie : la commande (ici get) + son paramètre (nom du fichier) + la taille du dit fichier s'il existe (sinon 0)
		tmpsizef = taille_fichier(f);
		sprintf(csizef,"% d\n",tmpsizef);
		strcat(commande, csizef);
		printf("Commande : (%zu) %s", strlen(commande), commande);
		nw = rio_writen(connfd, commande, strlen(commande)); // Envoi de toute la commande
		if (nw < 0) exit(1);

		printf("**************************************\n");	

		//GESTION D'ERREUR (envoyé par le serveur si le fichier est inexistant)
		if ( (n = rio_readlineb(&rio, err, MAXLINE)) > 0){
			err[strlen(err)-1]= '\0';
		}

		if ( strcmp( err , "ERR001") == 0 ){
			printf("ERREUR : Fichier inexistant. \n");

		}
		else {

			//TAILLE FICHIER : Reception de la taille du fichier
			if ( (n = rio_readlineb(&rio, csizef, MAXLINE)) > 0){
				sizef = atoi(csizef);
			}

			printf("Début du téléchargement de %s de taille %d / %d \n", linecmd[1], tmpsizef, sizef);

			assert(tmpsizef<=sizef);

			if (tmpsizef == sizef) {
				printf("Le fichier est déjà entièrement présent. \n");
				close(f);
			} 
			else {

				//Lecture de ce qui est envoyé par le serveur à PARTIR de tmpsizef	
				lseek(f, tmpsizef , SEEK_SET);
				char *buf = (char*)malloc(MAXLINE*sizeof(char));


				//Quelques calculs avant : 
				int nbpaquet = (sizef-tmpsizef)/MAXLINE;
				int restsizef = (sizef-tmpsizef) - nbpaquet*MAXLINE; //la taille restante à DL
				printf("A DL : %d paquets de taille MAXLINE + un paquet de taille %d \n", nbpaquet, restsizef);

				//réception des nbpaquets
				int i;
				for (i = 1; i <= nbpaquet; i++){

					memset (buf, 0, MAXLINE);

					if ( (n = rio_readn(connfd, buf, MAXLINE)) > 0){
						write(f, buf, strlen(buf));
						tmpsizef = taille_fichier(f);
						printf("Transfert en cours : %zu octets reçus. (%d sur %d)\n", strlen(buf), tmpsizef, sizef);

					}
					else if (n == 0){
						printf("Aucun octet lu... \n");
					}
					else {
						fprintf(stderr, "ERREUR : %s\n", strerror(errno));
						Close(connfd);
						exit(1);
					}

				}
				//réception du dernier paquet
				if (restsizef != 0){
					memset (buf, 0, MAXLINE);

					if ( (n = rio_readn(connfd, buf, restsizef)) > 0){
						write(f, buf, n);
						tmpsizef = taille_fichier(f);
						printf("Transfert en cours : %zu octets reçus. (%d sur %d)\n", n, tmpsizef, sizef);

					}
					else if (n == 0){
						printf("Aucun octet lu... \n");
					}
					else {
						fprintf(stderr, "ERREUR : %s\n", strerror(errno));
						Close(connfd);
						exit(1);
					}
				}
				printf("Transfert terminé. \n");
				free(buf);
				close(f);
			}	

			printf("**************************************\n");
		}
}
//--------------------------------------
//COMMANDE PUT : donne un fichier au serveur
int put(char linecmd[MAXCMD][MAXLINE], char commande[MAXLINE], int connfd){

	size_t n;
	ssize_t nw;
	rio_t rio;
	int f;
	int sizef = 0;
	char csizef[MAXLINE];
	int tmpsizef;
	
		//COMMANDE = linecmd[0] (ici get), NOM_FICHIER = linecmd[1], TAILLE_FICHIER = linecmd[2]
		f = open(linecmd[1], O_RDONLY);
		Rio_readinitb(&rio, connfd);
			
		//GESTION D'ERREUR
		if (f < 0){
			printf("ERREUR : Fichier inexistant. \n");
		}						
		else {					
			//ENVOI de la commande + la taille du fichier
			sizef = taille_fichier(f);
			sprintf(csizef," %d\n",sizef);
			strcat(commande, csizef);
			printf("Commande : (%zu) %s", strlen(commande), commande);
			nw = rio_writen(connfd, commande, strlen(commande)); // Envoi de toute la commande
			if (nw < 0) return -1;
			printf("Envoi du fichier %s de taille %d. \n", linecmd[1], sizef);
			
			//RECEPTION TAILLE du fichier du serveur
			memset (csizef, 0, MAXLINE);
			if ( (n = rio_readlineb(&rio, csizef, MAXLINE)) > 0){
				tmpsizef = atoi(csizef);
				printf("Taille reçue : %d \n", tmpsizef);
			}
						
			if (tmpsizef == sizef){
				printf("Le serveur possède déjà le fichier. \n");
			}
			else {
				printf("Lecture à partir de %d / %d... \n", tmpsizef, sizef);
				lseek(f, tmpsizef , SEEK_SET);
				
				//Quelques calculs avant : 
				int nbpaquet = (sizef-tmpsizef)/MAXLINE;
				int restsizef = (sizef-tmpsizef) - nbpaquet*MAXLINE; //la taille restante à DL
				printf("A DL : %d paquets de taille MAXLINE + un paquet de taille %d \n", nbpaquet, restsizef);
				char *buf = (char*)malloc(MAXLINE*sizeof(char));
				int i;
				
				//envoi des nbpaquets
				for (i = 1; i <= nbpaquet; i++){							
					sleep(1);
					memset (buf, 0, MAXLINE);
					
					n = rio_readn(f, buf, MAXLINE);
					nw = rio_writen(connfd, buf, MAXLINE);
					
					if  ( nw == -1 ){
						printf("ALERTE : La connexion du serveur semble interrompue. \n");
						return -1;
					}
					else {										
						printf("Envoi : (%zu) %zu octets \n", nw, n);
					}
				}
				//envoi du dernier paquet de taille restsizef
				if (restsizef != 0){
				
					sleep(1);
					memset (buf, 0, MAXLINE);
					n = rio_readn(f, buf, MAXLINE);
					nw = rio_writen(connfd, buf, restsizef);
					
					if  ( nw > 0 ){
						printf("Envoi : (%zu) %zu octets \n", nw, n);
					}
					else {
						printf("ALERTE : La connexion du serveur semble interrompue. \n");
						return -1;
					}
				}							
				
				printf("Fin de lecture. \n");
				free(buf);
				close(f);
			}
		}

	return 0;
}
//--------------------------------------
int main(int argc, char **argv)
{
	int clientfd, port_esclave, port_maitre;
	rio_t rio;
	ssize_t n, nw;
	char *host_maitre;
	char commande[MAXLINE], commande_cpy[MAXLINE];
	char linecmd[MAXCMD][MAXLINE];
	char chemin[MAXLINE]; //pour les commandes cd, ls etc...
	char liste_fichier[MAXLINE];
		
  if (argc < 2) {
      fprintf(stderr, "usage: %s <host>\n", argv[0]);
      exit(0);
  }
  
	printf("**************************************\n");
	host_maitre = argv[1];
	port_maitre = atoi(argv[2]);
	
	//Connecté au MAITRE en tant que client
	clientfd = Open_clientfd(host_maitre, port_maitre); 
	Rio_readinitb(&rio, clientfd);
	
	//récupération du port de l'esclave
	if (n = rio_readlineb(&rio, commande, MAXLINE) > 0){
				port_esclave = atoi(commande);
				printf("Redirection vers le port %d. \n", port_esclave);
	}
	
	close(clientfd);
	printf("**************************************\n");
	
	//Connexion au serveur esclave en tant que serveur
	int listenfd, connfd;
	socklen_t slavelen;
  struct sockaddr_in slaveaddr;
  char slave_ip_string[INET_ADDRSTRLEN];
  char slave_hostname[MAX_NAME_LEN];
	
	slavelen = (socklen_t)sizeof(slaveaddr);
	listenfd = Open_listenfd(port_esclave);	
	
	printf("Tentative de connexion avec l'esclave au port %d. \n", port_esclave);
	connfd = Accept(listenfd, (SA *)&slaveaddr, &slavelen);
	Getnameinfo((SA *) &slaveaddr, slavelen, slave_hostname, MAX_NAME_LEN, 0, 0, 0);
	Inet_ntop(AF_INET, &slaveaddr.sin_addr, slave_ip_string, INET_ADDRSTRLEN);
	
	printf("**************************************\n");
	printf("Client Connecté au serveur esclave. \n");
	Rio_readinitb(&rio, connfd);
	while(1){
	
		memset (linecmd, 0, MAXLINE); //réinitialise linecmd
		fflush(stdout);
		printf("ftp> ");
		
		//ENTREE COMMANDE + PARAMETRE : "get" pour DL un fichier, sinon "bye" pour quitter
		memset (commande, 0, MAXLINE);
		fgets(commande_cpy, MAXLINE, stdin); //Attention, '\n' est ajouté à la fin
		commande_cpy[strlen(commande_cpy)-1] = '\0';
		strcpy(commande, commande_cpy);
		split_line(commande_cpy, linecmd);
						
		if ( strcmp( linecmd[0] , "get") == 0 && strlen(linecmd[1]) != 0){
			
			get(linecmd, commande, connfd);	
		
		}
		else if ( strcmp( linecmd[0] , "bye") == 0 ){
			
				linecmd[0][strlen(linecmd[0])] = '\n';
				Rio_writen(connfd, linecmd[0], strlen(linecmd[0]));
				Close(connfd);
				printf("Terminé\n");
				return 0;
				
		}
		else if ( strcmp( linecmd[0] , "pwd") == 0 ){
			
				linecmd[0][strlen(linecmd[0])] = '\n';
				Rio_writen(connfd, linecmd[0], strlen(linecmd[0]));
				memset(chemin, 0, MAXLINE);
				if ( (n = rio_readlineb(&rio, chemin, MAXLINE)) > 0){
					chemin[n-1]= '\0';	
					printf("%s\n", chemin);
				}
				
		}
		else if ( strcmp( linecmd[0] , "cd") == 0 ){
			
				commande[strlen(commande)] = '\n';
				Rio_writen(connfd, commande, strlen(commande));
				
				
		}
		else if ( strcmp( linecmd[0] , "ls") == 0 ){
			
				linecmd[0][strlen(linecmd[0])] = '\n';
				Rio_writen(connfd, linecmd[0], strlen(linecmd[0]));
				memset(liste_fichier, 0, MAXLINE);
				if ( (n = rio_readlineb(&rio, liste_fichier, MAXLINE)) > 0){
					liste_fichier[n-1]= '\0';	
					printf("%s\n", liste_fichier);
				}
				
				
		}
		else if ( strcmp( linecmd[0] , "mkdir") == 0 ){
			
				commande[strlen(commande)] = '\n';
				Rio_writen(connfd, commande, strlen(commande));
				
				
		}
		else if ( strcmp( linecmd[0] , "rm") == 0 ){
			
				commande[strlen(commande)] = '\n';
				Rio_writen(connfd, commande, strlen(commande));
				
		}
		else if ( strcmp( linecmd[0] , "put") == 0 && strlen(linecmd[1]) != 0){
			
			put(linecmd, commande, connfd);	
		
		}
	}	
	
	return 0;
}
