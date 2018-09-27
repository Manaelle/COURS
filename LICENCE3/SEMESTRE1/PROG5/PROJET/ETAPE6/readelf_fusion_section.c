#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>

#include "readelf_type.h"

int fusion_section_write(int fd, elf donnees1, elf donnees2){
	
		int indSect;
		
	//ECRITURE DU HEADER
	write(fd, donnees1.ehdr->e_ident,      EI_NIDENT);
	write(fd, &donnees1.ehdr->e_type,      sizeof(donnees1.ehdr->e_type));
	write(fd, &donnees1.ehdr->e_machine,   sizeof(donnees1.ehdr->e_machine));
	write(fd, &donnees1.ehdr->e_version,   sizeof(donnees1.ehdr->e_version));
	write(fd, &donnees1.ehdr->e_entry,     sizeof(donnees1.ehdr->e_entry));
	write(fd, &donnees1.ehdr->e_phoff,     sizeof(donnees1.ehdr->e_phoff)); 
	write(fd, &donnees1.ehdr->e_shoff,     sizeof(donnees1.ehdr->e_shoff));
	write(fd, &donnees1.ehdr->e_flags,     sizeof(donnees1.ehdr->e_flags)); 
	write(fd, &donnees1.ehdr->e_ehsize,    sizeof(donnees1.ehdr->e_ehsize));
	write(fd, &donnees1.ehdr->e_phentsize, sizeof(donnees1.ehdr->e_phentsize));
	write(fd, &donnees1.ehdr->e_phnum,     sizeof(donnees1.ehdr->e_phnum));
	write(fd, &donnees1.ehdr->e_shentsize, sizeof(donnees1.ehdr->e_shentsize));
	write(fd, &donnees1.ehdr->e_shnum,     sizeof(donnees1.ehdr->e_shnum));
	write(fd, &donnees1.ehdr->e_shstrndx,  sizeof(donnees1.ehdr->e_shstrndx));
	
	//ECRITURE DU HEADER SECTION
      lseek(fd, donnees1.ehdr->e_shoff, SEEK_SET); 
      for(indSect = 0 ; indSect < donnees1.ehdr->e_shnum; indSect++){

          write(fd, &donnees1.shdrTable[indSect], sizeof(donnees1.shdrTable[indSect]));
      }

	
	return 0;
}
