#include "bit_operations.h"
#include "bit_fichier.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------
BFILE *bstart(FILE *fichier, const char *mode){
	
	BFILE *filebin;

	filebin = (BFILE *) malloc(sizeof(BFILE));
	
	if (strcmp(mode,"r")==0){
		filebin -> mode = 'r';
		filebin -> i = 0;		
		
  }
  else if (strcmp(mode,"w")==0){
  	filebin -> mode = 'w';
  	filebin -> i = 0;
		for(int i = 0; i < 8; i++){
			filebin -> buffer[i] = 0;
		}
  }
	
	return filebin;
}
//----------------------------------------------

int bstop(BFILE *fichier){
	
	//si l'octet est incomplet, on le remplit avec des zéro et on écrit le tout dans le fichier
	if ( fichier -> mode = 'w'){
		for(int i = (fichier -> i); i < 8; i++){
			fichier -> buffer[i] = 0;
		}
	fputc(fichier -> buffer, fichier -> bfichier);
	free(fichier);
	return 0;
}
//----------------------------------------------

int bitread(BFILE *fichier){


}
//----------------------------------------------

int bitwrite(BFILE *fichier, int bit){


}
//----------------------------------------------

int beof(BFILE *fichier){

}


