/*
 * echo - read and echo text lines until client closes connection
 */
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <assert.h>
#include "csapp.h"
#define MAXCMD 3

//--------------------------------------
//Retourne simplement la taille (en octet) du fichier
int taille_fichier(int fichier){
	
	long int s = 0;
	struct stat info;
	int status;

	if (fichier < 0){
		NULL; //en cas d'erreur, ne rien faire (sera traité dans echo() )
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
int get(char linecmd[MAXCMD][MAXLINE], int connfd){

	size_t n;
	ssize_t nw;
	int f;
	int sizef = 0;
	char csizef[MAXLINE];
	int tmpsizef;
	
						//COMMANDE = linecmd[0] (ici get), NOM_FICHIER = linecmd[1], TAILLE_FICHIER = linecmd[2]
						f = open(linecmd[1], O_RDONLY);
							
						//GESTION D'ERREUR
						if (f < 0){
							char err[MAXLINE] = "ERR001";
							printf("%s : Fichier inexistant \n", err);
							err[strlen(err)] = '\n';
							nw = rio_writen(connfd, err, strlen(err));
							if (nw < 0) return -1;
						}						
						else {
							//Le client attend de recevoir une erreur, l'envoi du SUCCESS est donc obligatoire
							char err[MAXLINE] = "SUCCESS";
							err[strlen(err)] = '\n';
							nw = rio_writen(connfd, err, strlen(err));
							if (nw < 0) return -1;
						
							//Envoi de la taille du fichier
							sizef = taille_fichier(f);
							sprintf(csizef,"%d\n",sizef);
							nw = rio_writen(connfd, csizef, strlen(csizef));
							if (nw < 0) return -1;
							
							printf("Demande de téléchargement du fichier %s de taille %d. \n", linecmd[1], sizef);
							
							tmpsizef = atoi(linecmd[2]);
							
							if (tmpsizef == sizef){
								printf("Le client possède déjà le fichier. \n");
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
										printf("ALERTE : La connexion du client semble interrompue. \n");
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
										printf("ALERTE : La connexion du client semble interrompue. \n");
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
//COMMANDE LS : liste tous les fichiers 
void ls(int connfd){	

	 struct dirent *lecture;
     DIR *rep;
	 char listfile[MAXLINE];
	 char file[100];
        
		memset(listfile, 0, MAXLINE);
		rep = opendir(".");
		
    while (lecture = readdir(rep)){
           
				printf("%s\n", lecture->d_name);
				sprintf(file,"%s ",lecture->d_name);
				strcat(listfile, file);
			
     }
		listfile[strlen(listfile)-1] = '\n';
		printf("%s", listfile);
		
		Rio_writen(connfd, listfile, strlen(listfile));
        closedir(rep);
}
//--------------------------------------
//COMMANDE CD : déplace seulement le processus ! (pas le serveur esclave)
void cd(char linecmd[MAXCMD][MAXLINE]){	

	if (chdir(linecmd[1]) < 0){
		fprintf(stderr, "%s\n", strerror(errno));
	}
}
//--------------------------------------
//COMMANDE PWD : affiche le chemin courant du processus
char *pwd(int connfd){	
	
	char chemin[MAXLINE];
	char *ptr;
	size_t nw;
	
	memset(chemin, 0, MAXLINE);
	if ((ptr = getcwd(chemin, MAXLINE)) < 0){
		fprintf(stderr, "%s\n", strerror(errno));
		return NULL;
	}
	else{
		printf("Chemin : %s\n", chemin);
		chemin[strlen(chemin)]='\n';
		nw = rio_writen(connfd, chemin, strlen(chemin));
		if (nw < 0) return NULL;
		return ptr;
	}
}
//--------------------------------------
//COMMANDE MKDIR : créer un dossier
void my_mkdir(char namedir[MAXLINE]){	
	
	char chemin[MAXLINE] = "./";
	
	if (strlen(namedir) <= 0){
		printf("ERREUR : nom de dossier incorrect. \n");
	}
	else {
		strcat(chemin, namedir);
		if (mkdir(chemin, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0){
			printf("ERREUR : création de dossier impossible. \n");
		}
	}
	
}
//--------------------------------------
int parcours(const char *chemin_fichier, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	
	int rv = remove(chemin_fichier);
	if (rv) perror(chemin_fichier);
	return rv;
	
}

//nftw parcourt récursivement TOUT l'arbrorescence à partir de chemin (ici le répertoire courant), 
//et y applique la fonction 'parcours' sur chaque fichier/dossier rencontré
int rm_all(char *chemin){
	
	return nftw(chemin, parcours, 10, FTW_DEPTH|FTW_PHYS);
	
}
//--------------------------------------
//COMMANDE RM : supprime un fichier ou un dossier
void my_rm(char linecmd[MAXCMD][MAXLINE]){		
	
	struct stat info;
	int status;

	//SUPPRESSION DE DOSSIER + du contenu
	if ( strcmp( linecmd[1] , "-r") == 0 ){		
		status = stat(linecmd[2], &info);
		if (S_ISDIR(info.st_mode) ){
			
			//cas d'un dossier plein
			if (rmdir(linecmd[2]) < 0 && errno == ENOTEMPTY ){
				printf("ATTENTION : Dossier non vide ! \n");
				char chemin[MAXLINE] = "./";
				strcat(chemin, linecmd[2]);
				rm_all(chemin);
			}
			else{
				printf("Dossier vide. Suppression faite... \n");
			}

		}
		else {
			printf("ATTENTION : utiliser rm pour un fichier classique. \n");
		}
	}
	
	//SUPPRESSION de fichier
	else {
		status = stat(linecmd[1], &info);
		if (S_ISREG(info.st_mode) ){
			if (remove(linecmd[1]) < 0){
			printf("ERREUR : suppression impossible. \n");
			}
		}
		else {
			printf("ATTENTION : utiliser rm -r pour un dossier ! \n");
		}
	}
	
}
//--------------------------------------
//COMMANDE PUT : reçoit un fichier venant du client 
int put(char linecmd[MAXCMD][MAXLINE], int connfd){

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
		
		//TAILLE TOTALE DU FICHIER
		sizef = atoi(linecmd[2]);

		//On envoie la taille actuel du fichier s'il existe déjà
		memset(csizef, 0, MAXLINE);
		tmpsizef = taille_fichier(f);
		sprintf(csizef,"%d\n",tmpsizef);
		nw = rio_writen(connfd, csizef, strlen(csizef));
		if (nw < 0) exit(1);

		printf("**************************************\n");	

		printf("Début du téléchargement de %s de taille %d / %d \n", linecmd[1], tmpsizef, sizef);

		assert(tmpsizef<=sizef);

		if (tmpsizef == sizef) {
			printf("Le fichier est déjà entièrement présent. \n");
			close(f);
		} 
		else {

			//Lecture de ce qui est envoyé par le client à PARTIR de tmpsizef	
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
//--------------------------------------
void errSocket(){
	
	return;

}
//--------------------------------------
int echo(int connfd)
{
	size_t n;
	size_t nw;
	char commande[MAXLINE];
	char linecmd[MAXCMD][MAXLINE];
	rio_t rio;
	int f;
	int sizef = 0;
	char csizef[MAXLINE];
	int tmpsizef;
	char *chemin;
	
  Rio_readinitb(&rio, connfd);
  signal(SIGPIPE, errSocket);
	
	do {
				memset (commande, 0, sizeof (commande));

				//LECTURE COMMANDE: lecture de commande. Tant que le client n'a pas tapé "bye", son traitement n'est pas fini.
				if ( (n = rio_readlineb(&rio, commande, MAXLINE)) != 0){	
					commande[n-1] ='\0';
					printf("Commande reçue : %s \n", commande);
					split_line(commande, linecmd);
				}
				else if (n==0) {
					return -1;
				}
				else {
					fprintf(stderr, "%s\n", strerror(errno));
					return -1;
				}
				
				//TRAITEMENT DE GET :
				if ( strcmp( linecmd[0] , "get") == 0 ){					
						
						if (get(linecmd, connfd) < 0) {return -1;}
				}
				
				//TRAITEMENT DU BYE : 
				else if ( strcmp( linecmd[0] , "bye" ) == 0 ){
				
					printf("*************\n");
					return 0;
				}
				else if ( strcmp( linecmd[0] , "cd" ) == 0 ){
				
					cd(linecmd);
					printf("Déplacement fait.\n");
					
				}
				else if ( strcmp( linecmd[0] , "pwd" ) == 0 ){				
					
					chemin = pwd(connfd);
					
				}
				else if ( strcmp( linecmd[0] , "ls" ) == 0 ){				
					
					ls(connfd);
					
				}
				else if ( strcmp( linecmd[0] , "mkdir" ) == 0 ){				
					
					my_mkdir(linecmd[1]);
					
				}
				else if ( strcmp( linecmd[0] , "rm" ) == 0 ){				
					
					printf("Suppression...\n");
					my_rm(linecmd);
					
				}
				else if ( strcmp( linecmd[0] , "put" ) == 0 ){				
					
					if (put(linecmd, connfd) < 0) {return -1;}
					
				}
				
				
		} while(1);
		
	  
}

