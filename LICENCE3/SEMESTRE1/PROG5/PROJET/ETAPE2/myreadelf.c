#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

#include "readelf_header.h"
#include "readelf_header_section.h"

int main(int argc, char *argv[])
    {
      int fd;
      Elf32_Ehdr *ehdr;
      Elf32_Shdr *shdrTable;
      char *tabNames;
      Elf32_Shdr stringTab;

      fd = open(argv[1], O_RDONLY); // Le dernier argument est le nom du fichier
      if(fd < 0)
      {
          fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", argv[1]);
          return 2;
      }

      //ETAPE 1
      ehdr = malloc(sizeof(Elf32_Ehdr));
      read_header(fd, ehdr);


      //ETAPE 2
      lseek(fd, ehdr->e_shoff + ehdr->e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
      read(fd, &stringTab, sizeof(stringTab));

      tabNames = malloc(stringTab.sh_size);

      lseek(fd, stringTab.sh_offset, SEEK_SET);//On récupère le nom de chaque section
      read(fd, tabNames, stringTab.sh_size);

      shdrTable = malloc(ehdr->e_shentsize * ehdr->e_shnum);
      read_sectionTable(fd, ehdr, shdrTable);
      print_sectionTable(ehdr, shdrTable, tabNames);
     

      close(fd);
      free(shdrTable);
      free(tabNames);
      free(ehdr);

      return 0;
    }
