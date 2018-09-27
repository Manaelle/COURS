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
#include "readelf_symbol.h"
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
	
	//Variables Etape 1
		int fd; 
		Elf32_Ehdr *ehdr;
	
	//Variables Etape 2
		Elf32_Shdr *shdrTable;
		char *tabNames;
		Elf32_Shdr stringTab;
		
	//Variables Etape 3
		int intSec, intArgv, retourInt;
		char* strSec, retourStr;
		
	//Variables Etape 4
		Elf32_Sym *symTable; 
		char *etape4Names; 
		Elf32_Shdr stringTab4; 
		int nbsymbole, decalsymtab, i, intSect4; 
	
	
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
  
  //ETAPE 4
		intSect4 = getIdFromName(tabNames, shdrTable, ".strtab", ehdr->e_shnum, error);
		lseek(fd, ehdr->e_shoff + intSect4 * sizeof(Elf32_Shdr), SEEK_SET);
		read(fd, &stringTab4, sizeof(stringTab4));
	 
		// on récupère le nom de chaque symbole table
		etape4Names = malloc(stringTab4.sh_size);
		lseek(fd, stringTab4.sh_offset, SEEK_SET);
		read(fd, etape4Names, stringTab4.sh_size);
	 
		// on recupere l'index de la table des symboles (dans le section header)
		i=0;
		while(i < ehdr->e_shnum && shdrTable[i].sh_type != SHT_SYMTAB){
		  i++;
		}
		if(i == ehdr->e_shnum){
		  printf("ERREUR : aucune table de symboles");
		  close(fd);
		  free(shdrTable);
		  free(tabNames);
		  free(ehdr);
		 	free(etape4Names);
		  return 0;
  }
 
  // on recupere le nb de symboles dans la table et le debut de la table dans le fichier
  nbsymbole = shdrTable[i].sh_size / shdrTable[i].sh_entsize;
  decalsymtab = shdrTable[i].sh_offset;
 
  symTable = malloc(sizeof(Elf32_Sym) * nbsymbole);
  read_symbole(fd, symTable, nbsymbole, decalsymtab);
  

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
  
  if (option4) {
		print_symbole(fd, etape4Names, symTable, nbsymbole);
  }
  
  close(fd);
  
  free(shdrTable);
  free(tabNames);
  free(ehdr);
  free(symTable);
  free(etape4Names);
  
	return 0;
}


