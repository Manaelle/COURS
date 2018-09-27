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
#include "readelf_header_section.h"

    //-------------------------------------------------------------------------------
    char *getType(Elf32_Shdr shdr){	
		//TYPE : Retourne de type de la section shdr
		
		const struct { Elf32_Word ind; char *str; } types[] =
		{
			{ SHT_NULL,           "NULL"           },
			{ SHT_PROGBITS,       "PROGBITS"       },
			{ SHT_SYMTAB,         "SYMTAB"         },
			{ SHT_STRTAB,         "STRTAB"         },
			{ SHT_RELA,           "RELA"           },
			{ SHT_HASH,           "HASH"           },
			{ SHT_DYNAMIC,        "DYNAMIC"        },
			{ SHT_NOTE,           "NOTE"           },
			{ SHT_NOBITS,         "NOBITS"         },
			{ SHT_REL,            "REL"            },
			{ SHT_SHLIB,          "SHLIB"          },
			{ SHT_DYNSYM,         "DYNSYM"         },
			{ SHT_INIT_ARRAY,     "INIT_ARRAY"     },
			{ SHT_FINI_ARRAY,     "FINI_ARRAY"     },
			{ SHT_PREINIT_ARRAY,  "PREINIT_ARRAY"  },
			{ SHT_GROUP,          "GROUP"          },
			{ SHT_SYMTAB_SHNDX,   "SYMTAB_SHNDX"   },
			{ SHT_GNU_ATTRIBUTES, "GNU_ATTRIBUTES" },
			{ SHT_GNU_HASH,       "GNU_HASH"       },
			{ SHT_GNU_LIBLIST,    "GNU_LIBLIST"    },
			{ SHT_GNU_verdef,     "VERDEF"         },
			{ SHT_GNU_verneed,    "VERNEED"        },
			{ SHT_GNU_versym,     "VERSYM"         },
			{ SHT_ARM_EXIDX,      "ARM_EXIDX"      },
			{ SHT_ARM_PREEMPTMAP, "ARM_PREEMPTMAP" },
			{ SHT_ARM_ATTRIBUTES, "ARM_ATTRIBUTES" },
			{ SHT_HIUSER,         "UNKNOWN"        }
		};
		int i;
		for (i =0; i<28; i++) {
			if (types[i].ind == shdr.sh_type) {
				return types[i].str;
			}
		}
		
    return "NULL";
    }

    //-------------------------------------------------------------------------------
    int read_sectionTable(int fd, elf *donnees)
    //READ_SECTIONTABLE : Récupération des informations lié à shdr
    {
      int indSect;

      lseek(fd, donnees->ehdr->e_shoff, SEEK_SET); //On place le curseur au début du section header table

      // On parcourt les headers de chaque section
      for(indSect = 0 ; indSect < donnees->ehdr->e_shnum; indSect++){

          //On récupère toute la section suivante
          read(fd, &donnees->shdrTable[indSect], sizeof(donnees->shdrTable[indSect]));
      }

      return 0;
    }
    
		//-------------------------------------------------------------------------------
		//PRINT_SECTIONTABLE : Affichage des informations lié à shdr
    int print_sectionTable(elf *donnees){
          char *name;
          char flags[4] = "";
          int indSect;
					
					printf("*************************************************\n");
          printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x:\n En-têtes de section :\n", donnees->ehdr->e_shnum, donnees->ehdr->e_shoff);

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

          for(indSect = 0 ; indSect < donnees->ehdr->e_shnum; indSect++){
          // On récupères les flags (se fait bit à bit et non pour le mot entier)
              strcpy(flags, "");
              if(donnees->shdrTable[indSect].sh_flags & SHF_WRITE)
                strcat(flags, "W");
              if(donnees->shdrTable[indSect].sh_flags & SHF_ALLOC)
                strcat(flags, "A");
              if(donnees->shdrTable[indSect].sh_flags & SHF_EXECINSTR)
                strcat(flags, "X");
              if(donnees->shdrTable[indSect].sh_flags &  SHF_MERGE)
                strcat(flags, "M");
              if(donnees->shdrTable[indSect].sh_flags &  SHF_STRINGS)
                strcat(flags, "S");
              if(donnees->shdrTable[indSect].sh_flags & SHF_INFO_LINK)
                strcat(flags, "I");
              if(donnees->shdrTable[indSect].sh_flags & SHF_LINK_ORDER)
                strcat(flags, "L");
              if(donnees->shdrTable[indSect].sh_flags & SHF_OS_NONCONFORMING)
                strcat(flags, "O");
              if(donnees->shdrTable[indSect].sh_flags & SHF_GROUP)
                strcat(flags, "G");
              if(donnees->shdrTable[indSect].sh_flags & SHF_TLS)
                strcat(flags, "T");
              if(donnees->shdrTable[indSect].sh_flags & SHF_MASKOS)
                strcat(flags, "o");
              if(donnees->shdrTable[indSect].sh_flags & SHF_ORDERED)
                strcat(flags, "x");
              if(donnees->shdrTable[indSect].sh_flags & SHF_EXCLUDE)
                strcat(flags, "E");
              if(donnees->shdrTable[indSect].sh_flags & SHF_MASKPROC)
                strcat(flags, "p");

              name = donnees->tabNames + donnees->shdrTable[indSect].sh_name;

              //On affiche les valeurs demandées
              printf(" [ %d] \t ", indSect);
              printf("%25.25s\t", name);
              printf("%15.15s \t", getType(donnees->shdrTable[indSect]));
              printf("%8.8x \t", donnees->shdrTable[indSect].sh_addr);
              printf("%6.6x \t", donnees->shdrTable[indSect].sh_offset);
              printf("%6.6x \t", donnees->shdrTable[indSect].sh_size);
              printf("%2.2x \t", donnees->shdrTable[indSect].sh_entsize);
              printf("%3s \t", flags);
              printf("  %d \t", donnees->shdrTable[indSect].sh_link);
              printf("  %d \t", donnees->shdrTable[indSect].sh_info);
              printf(" %d \n", donnees->shdrTable[indSect].sh_addralign);
          }

					printf("\n");
					
          return 0;
    }

