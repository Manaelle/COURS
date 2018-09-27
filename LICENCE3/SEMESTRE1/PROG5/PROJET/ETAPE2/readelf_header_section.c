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

    //-------------------------------------------------------------------------------
    char *getType(Elf32_Shdr shdr){	
		//TYPE : Retourne de type de la section shdr
		
      switch(shdr.sh_type){
      case 1:
          return "PROGBITS";
      case 2:
          return "SYMTAB ";
      case 3:
          return "STRTAB ";
      case 4:
          return "RELA  ";
      case 5:
          return "HASH  ";
      case 6:
          return "DYNAMIC";
      case 7:
          return "NOTE  ";
      case 8:
          return "NOBITS";
      case 9:
          return "REL  ";
      case 10:
          return "SHLIB";
      case 11:
          return "DYNSYM";
      case 0x70000000:
          return "LOPROC";
      case 0x7fffffff:
          return "HIPROC";
      case 0x80000000:
          return "LOUSER";
      case 0xffffffff:
          return "HIUSER";
      default:
          return "NULL  ";
      }
    }

    //-------------------------------------------------------------------------------
    int read_sectionTable(int fd, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrTable)
    //READ_SECTIONTABLE : Récupération des informations lié à shdr
    {
      int indSect;

      lseek(fd, ehdr->e_shoff, SEEK_SET); //On place le curseur au début du section header table

      // On parcourt les headers de chaque section
      for(indSect = 0 ; indSect < ehdr->e_shnum; indSect++){

          //On récupère toute la section suivante
          read(fd, &shdrTable[indSect], sizeof(shdrTable[indSect]));
      }

      return 0;
    }
    
		//-------------------------------------------------------------------------------
		//PRINT_SECTIONTABLE : Affichage des informations lié à shdr
    int print_sectionTable(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrTable, char *tabNames){
          char *name;
          char flags[4] = "";
          int indSect;
					
					printf("*************************************************\n");
          printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x:\n En-têtes de section :\n", ehdr->e_shnum, ehdr->e_shoff);

          printf(" [Nr]\t");
          printf("%25.25s\t","Nom");
          printf("%15.15s \t","Type");
          printf("%8.8s \t","Adr");
          printf("%6.6s \t","Offset");
          printf("%6.6s \t","Size");
          printf("%2.2s \t","ES");
          printf("%3.3s \t","Fan");
          
          printf("%3.3s \t","LN");
          printf("%3.3s \t","Inf");
          printf("%2.2s \n","Al");         

          for(indSect = 0 ; indSect < ehdr->e_shnum; indSect++){
          // On récupères les flags (se fait bit à bit et non pour le mot entier)
              strcpy(flags, "");
              if(shdrTable[indSect].sh_flags & SHF_WRITE)
                strcat(flags, "W");
              if(shdrTable[indSect].sh_flags & SHF_ALLOC)
                strcat(flags, "A");
              if(shdrTable[indSect].sh_flags & SHF_EXECINSTR)
                strcat(flags, "X");
              if(shdrTable[indSect].sh_flags &  SHF_MERGE)
                strcat(flags, "M");
              if(shdrTable[indSect].sh_flags &  SHF_STRINGS)
                strcat(flags, "S");
              if(shdrTable[indSect].sh_flags & SHF_INFO_LINK)
                strcat(flags, "I");
              if(shdrTable[indSect].sh_flags & SHF_LINK_ORDER)
                strcat(flags, "L");
              if(shdrTable[indSect].sh_flags & SHF_OS_NONCONFORMING)
                strcat(flags, "O");
              if(shdrTable[indSect].sh_flags & SHF_GROUP)
                strcat(flags, "G");
              if(shdrTable[indSect].sh_flags & SHF_TLS)
                strcat(flags, "T");
              if(shdrTable[indSect].sh_flags & SHF_MASKOS)
                strcat(flags, "o");
              if(shdrTable[indSect].sh_flags & SHF_ORDERED)
                strcat(flags, "x");
              if(shdrTable[indSect].sh_flags & SHF_EXCLUDE)
                strcat(flags, "E");
              if(shdrTable[indSect].sh_flags & SHF_MASKPROC)
                strcat(flags, "p");

              name = tabNames + shdrTable[indSect].sh_name;

              //On affiche les valeurs demandées
              printf(" [ %d] \t ", indSect);
              printf("%25.25s\t", name);
              printf("%15.15s \t", getType(shdrTable[indSect]));
              printf("%8.8x \t", shdrTable[indSect].sh_addr);
              printf("%6.6x \t", shdrTable[indSect].sh_offset);
              printf("%6.6x \t", shdrTable[indSect].sh_size);
              printf("%2.2x \t", shdrTable[indSect].sh_entsize);
              printf("%3s \t", flags);
              printf("  %d \t", shdrTable[indSect].sh_link);
              printf("  %d \t", shdrTable[indSect].sh_info);
              printf(" %d \n", shdrTable[indSect].sh_addralign);
          }

          printf("*************************************************\n");
					printf("\n");
					
          return 0;
    }
