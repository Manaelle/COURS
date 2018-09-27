#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
 
int main(int argc, char * argv[]){
	
	struct stat buffer;
	
	// IL MANQUE LES DROITS D'ACCES
	stat(argv[1], &buffer);
	
	printf("Donnée du fichier : \n");
	
	if (S_ISREG(buffer.st_mode)) { printf("Type : Fichier \n"); }
	else if (S_ISDIR(buffer.st_mode)) { printf("Type : Dossier \n"); }
	else { printf("Type : Inconnu \n"); }
	
	//printf("Périphérique : %s \n", &buffer.st_dev);
	//printf("Numéro i-noeud : %d \n", buffer.st_ino);
	//printf("Protection : %s \n", buffer.st_mode);
	//printf("Nb liens matériels : %d \n", buffer.st_nlink);
	//printf("UID propriétaire : %d \n", buffer.st_uid);
	//printf("GID propriétaire : %d \n", buffer.st_gid);
	//printf("Type périphérique : %s \n", &buffer.st_rdev);
	//printf("Taille totale en octets : %d \n", buffer.st_size);
	//printf("Taille de bloc pour E/S : %d \n", buffer.st_blksize);
	//printf("Nombre de blocs alloués : %d \n", buffer.st_blocks);
	
	/* Conversion en date */
	printf("Heure dernier accès : %s", ctime(&buffer.st_atime));
	printf("Heure dernière modification : %s", ctime(&buffer.st_mtime));
	printf("Heure dernier changement : %s", ctime(&buffer.st_ctime));
	
	

	return 0;
}


