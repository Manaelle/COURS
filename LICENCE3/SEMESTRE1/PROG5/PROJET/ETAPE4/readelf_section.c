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

//-------------------------------------------------------------------------------

int getIdFromName(char *tabNames, Elf32_Shdr *shdrTable, const char *name, int nbName, int error){
  int ind;

  for(ind = 1; ind < nbName; ind++){
      if(strcmp(tabNames + shdrTable[ind].sh_name, name) == 0){
        return ind;
      }
  }

  error = 1;
}

//-------------------------------------------------------------------------------

char *getNameFromId(char *tabNames, Elf32_Shdr *shdrTable, const int indSect, int nbName, int error){
  
  if(indSect > 0 && indSect <= nbName){
      return tabNames + shdrTable[indSect].sh_name;
  }

  error = 1;
}

//-------------------------------------------------------------------------------
	// PRINT_CONTENUSECTION : Lecture + Ecriture
int print_contenuSection(int fd, Elf32_Ehdr *ehdr, Elf32_Shdr *shdrTable, int ind, char *name, int error) 
{
		
		int section[shdrTable[ind].sh_size];
		
		
		//LECTURE
    lseek(fd, shdrTable[ind].sh_offset, SEEK_SET);

    int i = 0, j=0, k=0; //j'initialise le tab à 0

    while(i<shdrTable[ind].sh_size) {
        section[i] = 0;
      i++;
    }

    i = 0;
    
    while(i<shdrTable[ind].sh_size) {
        read(fd, &section[i], 1);
        i++;}
        
        
    //ECRITURE
 		printf("*************************************************\n");
    printf("Vidange hexadecimale de la section << %s >> : \n", name); //faut réussir à afficher le nom
    i=0;

    while(i<shdrTable[ind].sh_size){
        if(i%16==0){
            printf("\n 0x%8.8x", i);
        }
        if(i%4==0){
            printf(" ");
        }
        printf("%2.2x", section[i]);
        if(i%16==15){
            j=i-15;
            printf(" ");
            while(j<i){
                if(section[j] <= 32 || section[j] > 126 ){
                    printf(".");
                }
                else{
                    printf("%c", section[j]);
                }
                j++;
            }
        }

        i++;
    }
    
    //dernière ligne
    if(i%16!=15){
            j=i-(i%16);
            k=i;
            while(k%16!=0){ // ce while est pour s'aligner juste en dessous de la liste des caractères
                if(k%4==0){
                    printf("  ");
                }
                else{
                    printf("  ");
                }
                k++;
            }
            printf(" ");

            while(j<i){
                if(section[j] <= 32 || section[j] > 126 ){ //si c'est pas un caractere bah on met un point
                    printf(".");
                }
                else{
                    printf("%c", section[j]);
                }
                j++;
            }
            printf("\n");
    }

	printf("\n");
  return 0;
}


