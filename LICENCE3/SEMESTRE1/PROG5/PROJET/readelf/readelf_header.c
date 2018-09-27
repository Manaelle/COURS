#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>

#include "readelf_type.h"
#include "readelf_header.h"

int read_header(int fd, elf *donnees)
{
	
	read(fd, donnees->ehdr->e_ident, EI_NIDENT);
	if(donnees->ehdr->e_ident[0] != ELFMAG0 || donnees->ehdr->e_ident[1] != ELFMAG1 || donnees->ehdr->e_ident[2] != ELFMAG2 || donnees->ehdr->e_ident[3] != ELFMAG3)
	{
		fprintf(stderr, "Le fichier n'est pas de type ELF.\n");
		exit(3);
	}

	read(fd, &donnees->ehdr->e_type,      sizeof(donnees->ehdr->e_type));
	read(fd, &donnees->ehdr->e_machine,   sizeof(donnees->ehdr->e_machine));
	read(fd, &donnees->ehdr->e_version,   sizeof(donnees->ehdr->e_version));
	read(fd, &donnees->ehdr->e_entry,     sizeof(donnees->ehdr->e_entry));
	read(fd, &donnees->ehdr->e_phoff,     sizeof(donnees->ehdr->e_phoff)); 
	read(fd, &donnees->ehdr->e_shoff,     sizeof(donnees->ehdr->e_shoff));
	read(fd, &donnees->ehdr->e_flags,     sizeof(donnees->ehdr->e_flags)); 
	read(fd, &donnees->ehdr->e_ehsize,    sizeof(donnees->ehdr->e_ehsize));
	read(fd, &donnees->ehdr->e_phentsize, sizeof(donnees->ehdr->e_phentsize));
	read(fd, &donnees->ehdr->e_phnum,     sizeof(donnees->ehdr->e_phnum));
	read(fd, &donnees->ehdr->e_shentsize, sizeof(donnees->ehdr->e_shentsize));
	read(fd, &donnees->ehdr->e_shnum,     sizeof(donnees->ehdr->e_shnum));
	read(fd, &donnees->ehdr->e_shstrndx,  sizeof(donnees->ehdr->e_shstrndx));

	return 0;
}

//-------------------------------------------------------------------------------
int print_header(elf *donnees)
// PRINT_HEADER : Affiche tous les éléments de ehdr.

{
	printf("***************************************************\n");
	printf("Mot magique : %u %c %c %c \n \n ", donnees->ehdr->e_ident[0], donnees->ehdr->e_ident[1], donnees->ehdr->e_ident[2], donnees->ehdr->e_ident[3]);
	
	//CLASSE
	switch(donnees->ehdr->e_ident[EI_CLASS]){
		case ELFCLASS32 : 
			printf("Classe : ELF32 \n");
			break;
		case ELFCLASS64 : 
			printf("Classe : ELF64 \n");
			break;
		}

	//DONNEE
	switch(donnees->ehdr->e_ident[EI_DATA]){
		case ELFDATA2LSB : 
			printf("Donnée : Little Endian \n");
			break;
		case ELFDATA2MSB : 
			printf("Donnée : Big Endian\n");
			break;
		}

	//VERSION
	switch(donnees->ehdr->e_ident[EI_VERSION]){
		case EV_CURRENT : 
			printf("Version : Courant (1) \n");
			break;
		}

	//TYPE
	switch(donnees->ehdr->e_type){
		case ET_REL : 
			printf("Type : Fichier de Relocalisation \n");
			break;
		case ET_EXEC : 
			printf("Type : Fichier d'Exécution \n");
			break;
		case ET_DYN : 
			printf("Type : Fichier d'Objet Partagé \n");
			break;
		case ET_CORE : 
			printf("Type : Fichier Core \n");
			break;
		case ET_LOPROC | ET_HIPROC : 
			printf("Type : Fichier spécifique au Processeur \n");
			break;
		}

	//MACHINE
	switch(donnees->ehdr->e_machine){
		case EM_M32 : 
			printf("Machine : AT&T WE 32100 \n");
			break;
		case EM_SPARC : 
			printf("Machine : SPARC \n");
			break;
		case EM_386 : 
			printf("Machine : Intel 80386 \n");
			break;
		case EM_68K : 
			printf("Machine : Motorola 68000 \n");
			break;
		case EM_88K : 
			printf("Machine : Motorola 88000 \n");
			break;
		case EM_860 : 
			printf("Machine : Intel 80860 \n");
			break;
		case EM_MIPS : 
			printf("Machine : MIPS RS3000 \n");
			break;
		case EM_ARM : 
			printf("Machine : ARM \n");
			break;
		default :
			printf("Machine : Inconnu (%d) \n", donnees->ehdr->e_machine);
		}

 //ADRESSE DU POINT D'ENTREE
  printf("Adresse de point d'entrée : %"PRIu32 "\n", donnees->ehdr->e_entry);

  //DEBUT D'EN-TETE DU PROGRAMME
  printf("Début des en-têtes de programme : %lu octets \n", (unsigned long)donnees->ehdr->e_phoff);

  //DEBUT D'EN-TETE DE SECTION
  printf("Début des en-têtes de section: %lu octets \n", (unsigned long)donnees->ehdr->e_shoff);

  //FANIONS
  printf("Fanions : %x \n", donnees->ehdr->e_flags);

  //TAILLE DE L'EN-TETE
  printf("Taille cet en-tête : %u bytes \n", (unsigned)donnees->ehdr->e_ehsize);

  //TAILLE DE L'EN-TETE DU PROGRAMME
  printf("Taille de l'en-tête du programme : %u octets \n", (unsigned)donnees->ehdr->e_phentsize);

  //TAILLE DE L'EN-TETE DE SECTION
  printf("Nombre d'en-têtes du programme : %u \n", (unsigned)donnees->ehdr->e_phnum);

  //TAILLE DE L'EN-TETE DE SECTION
  printf("Taille des en-têtes de section : %u octets \n", (unsigned)donnees->ehdr->e_shentsize);

  //NOMBRE D'EN-TETES DE SECTIONS
  printf("Nombre d'en-têtes de section : %u \n", (unsigned)donnees->ehdr->e_shnum);

  //TABLES D'INDEXES DES CHAINES D'EN-TETE DE SECTIONS
  printf("Table d'indexes des chaînes d'en-tête de section : %u bytes \n", (unsigned)donnees->ehdr->e_shstrndx);
  
  printf("\n");


	return 0;
}


