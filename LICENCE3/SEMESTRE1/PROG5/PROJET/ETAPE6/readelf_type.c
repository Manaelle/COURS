#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>

#include "readelf_header.h"
#include "readelf_header_section.h"
#include "readelf_section.h"
#include "readelf_symbol.h"
#include "readelf_reimp.h"
#include "readelf_type.h"
#include "debug.h"

elf read_all(int fd, int *error){


	//Variables utiles pour l'étape 4
		int i, intSect4;
		elf donnees;

				
	//ETAPE 1
		donnees.ehdr = malloc(sizeof(Elf32_Ehdr));
		read_header(fd, donnees.ehdr);

  //ETAPE 2
		lseek(fd, donnees.ehdr->e_shoff + donnees.ehdr->e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
		read(fd, &donnees.stringTab, sizeof(donnees.stringTab));

		donnees.tabNames = malloc(donnees.stringTab.sh_size);

		lseek(fd, donnees.stringTab.sh_offset, SEEK_SET);  // on récupère le nom de chaque section
		read(fd, donnees.tabNames, donnees.stringTab.sh_size);

		donnees.shdrTable = malloc(donnees.ehdr->e_shentsize * donnees.ehdr->e_shnum);
		read_sectionTable(fd, donnees.ehdr, donnees.shdrTable);

  //ETAPE 3 : Voir menu car lecture non obligatoire

  //ETAPE 4
		intSect4 = getIdFromName(donnees.tabNames, donnees.shdrTable, ".strtab", donnees.ehdr->e_shnum, error);
		lseek(fd, donnees.ehdr->e_shoff + intSect4 * sizeof(Elf32_Shdr), SEEK_SET);
		read(fd, &donnees.stringTab4, sizeof(donnees.stringTab4));

		// on récupère le nom de chaque symbole table
		donnees.etape4Names = malloc(donnees.stringTab4.sh_size);
		lseek(fd, donnees.stringTab4.sh_offset, SEEK_SET);
		read(fd, donnees.etape4Names, donnees.stringTab4.sh_size);

		// on recupere l'index de la table des symboles (dans le section header)
		i=0;
		while(i < donnees.ehdr->e_shnum && donnees.shdrTable[i].sh_type != SHT_SYMTAB){
		  i++;
		}
		
		if(i == donnees.ehdr->e_shnum){
		  printf("ERREUR : aucune table de symboles");
		  close(fd);
		  free(donnees.shdrTable);
		  free(donnees.tabNames);
		  free(donnees.ehdr);
      free(donnees.etape4Names);
		  exit(0);
  }

  // on recupere le nb de symboles dans la table et le debut de la table dans le fichier
  donnees.nbsymbole = donnees.shdrTable[i].sh_size / donnees.shdrTable[i].sh_entsize;
  donnees.decalsymtab = donnees.shdrTable[i].sh_offset;

  donnees.symTable = malloc(sizeof(Elf32_Sym) * donnees.nbsymbole);
  read_symbole(fd, donnees.symTable, donnees.nbsymbole, donnees.decalsymtab);

  //ETAPE 5
  donnees.indSecRel = 0;
  donnees.nbentreeSecRel = 0;
  
  return donnees;
  
  }
