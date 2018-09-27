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

    
    //-------------------------------------------------------------------------------
    //READ_SYMBOL : va lire la table des symboles
    int read_symbole(int fd, Elf32_Sym *symTable, int nbsymbole, int decalsymtab){
    
    int i=0;
   
    lseek(fd, decalsymtab, SEEK_SET);
    
    while(i<nbsymbole){
        read(fd, &symTable[i], sizeof(symTable[i]));
        i++;
    }

    return 0;
}
    
    //-------------------------------------------------------------------------------
    //PRINT_SYMBOL : affiche la table des symboles
    int print_symbole(int fd, char *etape4Names, Elf32_Sym *symTable, int nbsymbole){
    int i=0, type, bind;
    char *name;

    printf("*************************************************\n");
    printf("Table de symboles << .symtab >> contient %d entrees:\n", nbsymbole);
    
    			printf("Num: \t");
          printf("%-8s \t","Valeur");
          printf("%-4s \t","Tail");
          printf("%-8s \t","Type");
          printf("%-8s \t","Lien");
          printf("%-10s \t","Vis");
          printf("%-3s \t","Ndx");
          printf("%-25s \n","Nom");
          

   
    while(i<nbsymbole){
        
        printf("%i :\t", i); //Num
        printf("%8.8x \t", symTable[i].st_value); //Valeur
        printf("%-4d \t", symTable[i].st_size); //Tail
        
        type = ((symTable[i].st_info)&0xf);
        
        //type
        switch (type){
		      case STT_NOTYPE:
		          printf("%-8s \t", "NOTYPE");
		          break;
		      case STT_OBJECT:
		          printf("%-8s \t", "OBJECT");
		          break;
		      case STT_FUNC:
		          printf("%-8s \t","FUNC");
		          break;
		      case STT_SECTION:
		          printf("%-8s \t", "SECTION");
		          break;
		      case STT_FILE:
		          printf("%-8s \t", "FILE");
		          break;
		      case STT_LOPROC:
		          printf("%-8s \t", "LOPROC");
		          break;
		      case STT_HIPROC:
		          printf("%-8s \t", "HIPROC");
		          break;
        }
				
				//lien
        bind = ((symTable[i].st_info)>>4);
        switch (bind){
		      case STB_LOCAL:
		          printf("%-8s \t","LOCAL");
		          break;
		      case STB_GLOBAL:
		          printf("%-8s \t","GLOBAL");
		          break;
		      case STB_WEAK:
		          printf("%-8s \t","WEAK");
		          break;
		      case STB_LOPROC:
		          printf("%-8s \t","LOPROC");
		          break;
		      case STB_HIPROC:
		          printf("%-8s \t","HIPROC");
		          break;
        }

      switch (symTable[i].st_other){
		    case STV_DEFAULT :
		      printf("%-10s \t","DEFAULT");
		      break;
		    case STV_INTERNAL :
		      printf("%-10s \t","INTERNAL");
		      break;
		    case STV_HIDDEN :
		      printf("%-10s \t","HIDDEN");
		      break;
		    case STV_PROTECTED :
		      printf("%-10s \t","PROTECTED");
		      break;
		    default :
		      printf("%-10s \t","ERREUR");
		      break;
      }
				
				//Vis
       switch (symTable[i].st_shndx){
		      case SHN_UNDEF:
		          printf("%-3s \t","UND");
		          break;
		      case SHN_ABS:
		          printf("%-3s \t","ABS");
		          break;
		      default:
		          printf("%3.3d \t", symTable[i].st_shndx);
		          break;
        }
        
        name = etape4Names + symTable[i].st_name;
        printf("%-25s \t", name); //Nom
        printf("\n");

        i++;
    }

    return 0;
}


