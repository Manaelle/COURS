#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>

#include "readelf_header.h"
#include "readelf_type.h"

int read_header(int fd, Elf32_Ehdr *ehdr)
{
	
	read(fd, ehdr->e_ident, EI_NIDENT);
	if(ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 || ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3)
	{
		fprintf(stderr, "Le fichier n'est pas de type ELF.\n");
		exit(3);
	}

	read(fd, &ehdr->e_type,      sizeof(ehdr->e_type));
	read(fd, &ehdr->e_machine,   sizeof(ehdr->e_machine));
	read(fd, &ehdr->e_version,   sizeof(ehdr->e_version));
	read(fd, &ehdr->e_entry,     sizeof(ehdr->e_entry));
	read(fd, &ehdr->e_phoff,     sizeof(ehdr->e_phoff)); 
	read(fd, &ehdr->e_shoff,     sizeof(ehdr->e_shoff));
	read(fd, &ehdr->e_flags,     sizeof(ehdr->e_flags)); 
	read(fd, &ehdr->e_ehsize,    sizeof(ehdr->e_ehsize));
	read(fd, &ehdr->e_phentsize, sizeof(ehdr->e_phentsize));
	read(fd, &ehdr->e_phnum,     sizeof(ehdr->e_phnum));
	read(fd, &ehdr->e_shentsize, sizeof(ehdr->e_shentsize));
	read(fd, &ehdr->e_shnum,     sizeof(ehdr->e_shnum));
	read(fd, &ehdr->e_shstrndx,  sizeof(ehdr->e_shstrndx));

	return 0;
}

//-------------------------------------------------------------------------------
int print_header(Elf32_Ehdr *ehdr)
// PRINT_HEADER : Affiche tous les éléments de ehdr.

{
	printf("***************************************************\n");
	printf("Mot magique : %u %c %c %c \n \n ", ehdr->e_ident[0], ehdr->e_ident[1], ehdr->e_ident[2], ehdr->e_ident[3]);
	
	//CLASSE
	switch(ehdr->e_ident[EI_CLASS]){
		case ELFCLASS32 : 
			printf("Classe : ELF32 \n");
			break;
		case ELFCLASS64 : 
			printf("Classe : ELF64 \n");
			break;
		}

	//DONNEE
	switch(ehdr->e_ident[EI_DATA]){
		case ELFDATA2LSB : 
			printf("Donnée : Little Endian \n");
			break;
		case ELFDATA2MSB : 
			printf("Donnée : Big Endian\n");
			break;
		}

	//VERSION
	switch(ehdr->e_ident[EI_VERSION]){
		case EV_CURRENT : 
			printf("Version : Courant (1) \n");
			break;
		}

	//TYPE
	switch(ehdr->e_type){
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
	switch(ehdr->e_machine){
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
			printf("Machine : Inconnu (%d) \n", ehdr->e_machine);
		}

 //ADRESSE DU POINT D'ENTREE
  printf("Adresse de point d'entrée : %"PRIu32 "\n", ehdr->e_entry);

  //DEBUT D'EN-TETE DU PROGRAMME
  printf("Début des en-têtes de programme : %lu octets \n", (unsigned long)ehdr->e_phoff);

  //DEBUT D'EN-TETE DE SECTION
  printf("Début des en-têtes de section: %lu octets \n", (unsigned long)ehdr->e_shoff);

  //FANIONS
  printf("Fanions : %x \n", ehdr->e_flags);

  //TAILLE DE L'EN-TETE
  printf("Taille cet en-tête : %u bytes \n", (unsigned)ehdr->e_ehsize);

  //TAILLE DE L'EN-TETE DU PROGRAMME
  printf("Taille de l'en-tête du programme : %u octets \n", (unsigned)ehdr->e_phentsize);

  //TAILLE DE L'EN-TETE DE SECTION
  printf("Nombre d'en-têtes du programme : %u \n", (unsigned)ehdr->e_phnum);

  //TAILLE DE L'EN-TETE DE SECTION
  printf("Taille des en-têtes de section : %u octets \n", (unsigned)ehdr->e_shentsize);

  //NOMBRE D'EN-TETES DE SECTIONS
  printf("Nombre d'en-têtes de section : %u \n", (unsigned)ehdr->e_shnum);

  //TABLES D'INDEXES DES CHAINES D'EN-TETE DE SECTIONS
  printf("Table d'indexes des chaînes d'en-tête de section : %u bytes \n", (unsigned)ehdr->e_shstrndx);
  
  printf("\n");


	return 0;
}


