#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <getopt.h>

#include "readelf_header.h"
#include "readelf_header_section.h"
#include "readelf_section.h"
#include "debug.h"

int error = 0;
//--------------------------------------------
void usage(char *name) {
	fprintf(stderr, "Usage %s :\n"
		" -h : Affichage du header \n -s : Affichage du header de sections \n -x [nom_section]: Affichage de la section nom_section \n -S : Affichage de la table des symboles \n -r : Affichage de relocation \n" 
		, name);
}
//--------------------------------------------
int main(int argc, char *argv[]) {

	int opt;
	char *option3;
	int option1, option2, option4, option5;
	
	// DECLARATIONS
	
	int fd;
  Elf32_Ehdr *ehdr;
  
  Elf32_Shdr *shdrTable;
  char *tabNames;
  Elf32_Shdr stringTab;
  
  int intSec, intArgv, retourInt;
  char* strSec, retourStr;
	
	
	struct option longopts[] = {
		{ "debug", required_argument, NULL, 'd' },
		{ "option1", required_argument, NULL, 'h' },
		{ "option2", required_argument, NULL, 's' },
		{ "option3", required_argument, NULL, 'x' },
		{ "option4", required_argument, NULL, 'S' },
		{ "option5", required_argument, NULL, 'r' },
		{ "help", no_argument, NULL, 'H' },
		{ NULL, 0, NULL, 0 }
	};

	option1 = 0;
	option2 = 0;
	option3 = NULL;
	option4 = 0;
	option5 = 0;
	
	
	while ((opt = getopt_long(argc, argv, "hsSr:x:d:H", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			option1 = 1;
			break;
		case 's':
			option2 = 1;
			break;
		case 'x':
			option3 = optarg;
			break;
		case 'S':
			option4 = 1;
			break;
		case 'r':
			option5 = 1;
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
			  return 0;
		}
	} else {
		printf("ERREUR : Pas d'argument.");
		return 0;
	}
	

  
  //ETAPE 1
  ehdr = malloc(sizeof(Elf32_Ehdr));
  read_header(fd, ehdr);
 
  //ETAPE 2
  lseek(fd, ehdr->e_shoff + ehdr->e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
  read(fd, &stringTab, sizeof(stringTab));

  tabNames = malloc(stringTab.sh_size);
  
  lseek(fd, stringTab.sh_offset, SEEK_SET);  // on récupère le nom de chaque section
  read(fd, tabNames, stringTab.sh_size);

  shdrTable = malloc(ehdr->e_shentsize * ehdr->e_shnum);
  read_sectionTable(fd, ehdr, shdrTable);
  
  //ETAPE 3 : Voir menu car lecture non obligatoire
  

	//AFFICHAGE SELON OPTIONS
  
  if (option1) {
		print_header(ehdr);
  }

  if (option2) {
		print_sectionTable(ehdr, shdrTable, tabNames);
  }
  
  if (option3 != NULL) {
  
		intArgv = atoi(option3);
		
		if(intArgv == 0){ // si ce n'est pas un entier (le nom)
		      strSec = (char *)option3;
		      intSec = getIdFromName(tabNames, shdrTable, option3, ehdr->e_shnum, error);
		      } 
		      
		 else {
		      intSec = intArgv;
		      strSec = getNameFromId(tabNames, shdrTable, intSec, ehdr->e_shnum, error);
		      }
        
  	if(!error){
       	print_contenuSection(fd, ehdr, shdrTable, intSec, strSec, error); //Pour l'instant ça prend en compte que le numéro qu'on met ici
        } 
  else {
        printf("ERREUR : Section inexistante.");
        error = 0;
        }
        
  }
  
  close(fd);
  free(shdrTable);
  free(tabNames);
  free(ehdr);
  
	return 0;
}

//--------------------------------------------------------------

/*int main(int argc, char *argv[])
{
  int fd;
  Elf32_Ehdr *ehdr;
  Elf32_Shdr *shdrTable;
  char *tabNames;
  Elf32_Shdr stringTab;
  int intSec, intArgv, retourInt;
  char* strSec, retourStr;
  
  //MENU
  if (argc < 3 || (argc < 4 && strcmp(argv[1], "-x") == 0) || argc > 4 || (argc > 3 && strcmp(argv[1], "-x") != 0) || argv[1][0] != '-') {
   printf("LE PREMIER\n");
    help();
   return 0;
  }

  fd = open(argv[argc-1], O_RDONLY); // Le dernier argument est le nom du fichier
 
  if(fd < 0)
  {
      fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", argv[argc-1]);
      return 2;
  }

  //ETAPE 1
  ehdr = malloc(sizeof(Elf32_Ehdr));
  read_header(fd, ehdr);
 
  //ETAPE 2
  lseek(fd, ehdr->e_shoff + ehdr->e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
  read(fd, &stringTab, sizeof(stringTab));

  tabNames = malloc(stringTab.sh_size);

  // on récupère le nom de chaque section
  lseek(fd, stringTab.sh_offset, SEEK_SET);
  read(fd, tabNames, stringTab.sh_size);

  shdrTable = malloc(ehdr->e_shentsize * ehdr->e_shnum);
  read_sectionTable(fd, ehdr, shdrTable);
 
  // fin etape 2

  // debut etape 3
 
 
  // fin etape 3


  // etape 4
  // fin etape 4


  // etape 5
  // fin etape 5

   if (strcmp(argv[1], "-h") == 0) {
      print_header(ehdr);
   } else if (strcmp(argv[1], "-S") == 0) {
      print_sectionTable(ehdr, shdrTable, tabNames);
   } else if (strcmp(argv[1], "-x") == 0) {
      intArgv = atoi(argv[2]);
        if(intArgv == 0){ // si ce n'est pas un entier (le nom)
         strSec = (char *) argv[2];
         intSec = getIdFromName(tabNames, shdrTable, argv[2], ehdr->e_shnum);   
        } else {
         intSec = intArgv;
         strSec = getNameFromId(tabNames, shdrTable, intSec, ehdr->e_shnum);
        }
      if(!error){
         print_contenuSection(fd, ehdr, shdrTable, intSec, strSec); //Pour l'instant ça prend en compte que le numéro qu'on met ici
        } else {
         printf("erreur section inexistante");
         error = 0;
        }
   } else if (strcmp(argv[1], "-s") == 0) {
      //AFFICHER 4
   } else if (strcmp(argv[1], "-r") == 0) {
      //AFFICHER 5
   } else {
      help();
   }
 
  close(fd);
 
  free(shdrTable);
  free(tabNames);
  free(ehdr);

  return 0;
} */
