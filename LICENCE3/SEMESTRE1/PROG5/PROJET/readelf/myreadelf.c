#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <getopt.h>


#include "readelf_type.h"
#include "readelf_header.h"
#include "readelf_header_section.h"
#include "readelf_section.h"
#include "readelf_symbol.h"
#include "readelf_reimp.h"
#include "debug.h"

//--------------------------------------------

void usage(char *name) {
	fprintf(stderr, "Usage %s :\n"
		" -h : Affichage du header \n -s : Affichage du header de sections \n -x [nom_section]: Affichage de la section nom_section \n -S : Affichage de la table des symboles \n -r : Affichage de relocation \n"
		, name);
}
//--------------------------------------------
int main(int argc, char *argv[]) {

	int opt;
	char *option3, *option6;
	int option1, option2, option4, option5;

	// DECLARATIONS

	//Variables générales
		int fd, fd2/*, fd3*/;
		elf donnees, donnees2;
		int *error;

	//Variables utiles pour l'étape 3
	  int intSec, intArgv;

	//Variables utiles pour l'étape 4
		//int i;

	error = malloc(sizeof(int));
	*error = 0;


	struct option longopts[] = {
		{ "debug", required_argument, NULL, 'd' },
		{ "option1", required_argument, NULL, 'h' },
		{ "option2", required_argument, NULL, 'S' },
		{ "option3", required_argument, NULL, 'x' },
		{ "option4", required_argument, NULL, 's' },
		{ "option5", required_argument, NULL, 'r' },
		{ "option6", required_argument, NULL, 'f' },
		{ "help", no_argument, NULL, 'H' },
		{ NULL, 0, NULL, 0 }
	};

	option1 = 0;
	option2 = 0;
	option3 = NULL;
	option4 = 0;
	option5 = 0;
	option6 = NULL;


	while ((opt = getopt_long(argc, argv, "hsSrf:x:d:H", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			option1 = 1;
			break;
		case 'S':
			option2 = 1;
			break;
		case 'x':
			option3 = optarg;
			break;
		case 's':
			option4 = 1;
			break;
		case 'r':
			option5 = 1;
			break;
		case 'f':
			option6 = optarg;
			break;
		case 'H':
			usage(argv[0]);
			exit(0);
		case 'd':
			add_debug_to(optarg);
			break;
		default:
			fprintf(stderr, "ERREUR : Option %c non reconnu.\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}
	if (argc >= 2) {
		fd = open(argv[argc-1], O_RDONLY); // Le dernier argument est le nom du fichier

		if(fd < 0)
		{
				 fprintf(stderr, "ERREUR : Impossible d'ouvrir le fichier %s.\n", argv[argc-1]);
			  exit(0);
		}
	} else {
		printf("ERREUR : Pas d'argument.");
		exit(0);
	}

	//LECTURE

 	donnees = read_all(fd, error);

	//AFFICHAGE SELON OPTIONS

  if (option1) {
		print_header(&donnees);
  }

  if (option2) {
		print_sectionTable(&donnees);
  }

  if (option3 != NULL) {

		intArgv = atoi(option3);

		if(intArgv == 0){ // si ce n'est pas un entier (le nom)
		      donnees.strSec = (char *)option3;
		      intSec = getIdFromName(donnees.tabNames, donnees.shdrTable, option3, donnees.ehdr->e_shnum, error);
		      }

		 else {
		      intSec = intArgv;
		      donnees.strSec = getNameFromId(donnees.tabNames, donnees.shdrTable, intSec, donnees.ehdr->e_shnum, error);
		      }

  	if(*error == 0){
       	print_contenuSection(fd, &donnees, intSec, donnees.strSec); //Pour l'instant ça prend en compte que le numéro qu'on met ici
        }
  else {
        printf("ERREUR : Section inexistante. \n");
        exit(0);
        }

  }

  if (option4) {
		print_symbole(fd, &donnees);
  }

  if (option5) {
        print_reimp(fd, &donnees);
  }
  
  if (option6 != NULL) {
   
				 // FICHIER 2
				 if (argc >= 3) {
						fd2 = open(argv[argc -2], O_RDONLY); // Le dernier argument est le nom du fichier
						
						if(fd2 < 0)
						{
						       fprintf(stderr, "ERREUR : Impossible d'ouvrir le deuxième fichier %s.\n", argv[argc-2]);
						     exit(0);
						}
				 } else {
						printf("ERREUR : Pas d'argument.");
						exit(0);
				 }

					 //ETAPE LECTURE
				 	 donnees2 = read_all(fd2, error);
					 
					 free(donnees2.shdrTable);
					 free(donnees2.tabNames);
					 free(donnees2.ehdr);
					 free(donnees2.symTable);
					 free(donnees2.etape4Names);
  }
  
  
  close(fd);

  free(donnees.shdrTable);
  free(donnees.tabNames);
  free(donnees.ehdr);
  free(donnees.symTable);
  free(donnees.etape4Names);
  free(donnees->rel);
  free(error);

	return 0;
}


