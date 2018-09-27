#include <stdio.h>
#include <string.h>
#include "arbrebin.h"
#include "bfile.h"
#include "assert.h"

FILE * fichier_encode;

Arbre ArbreHuffman;

//--------------------------------------------------------
void Decoder() {

	int b;
	Arbre AC = ArbreVide();
	
	//DECODAGE : RECUPERATION DE L'ARBRE DE HUFFMAN
	ArbreHuffman = ArbreVide();
	ArbreHuffman = LireArbre(fichier_encode);

	//DECODAGE : RECUPERATION DU FICHIER CODE
	BFILE *bfichier = bstart(fichier_encode, "r");	
	
	AC = ArbreHuffman;
	
	//lecture des bits du texte encodé.
	do{				
		b = bitread(bfichier);
		
		if (b == 1){
			AC = FilsGauche(AC);
		}
		else {
			AC = FilsDroit(AC);
		}		
		//si AC est une feuille, alors on a notre lettre et on remonte à la racine de l'arbre
		if( EstVide(FilsDroit(AC)) && EstVide(FilsGauche(AC)) && (b != -1) ){
			
			printf("%c", (char)Etiq(AC));
			AC = ArbreHuffman;
		}
		
		// si b = -1, on a atteint la fin du fichier.
		if (b == -1) {
			AC = ArbreHuffman;			
		}
		
	}while(!beof(bfichier));
	
	if (AC != ArbreHuffman){
		printf("ERREUR : Le texte n'a pas été entièrement décodé...\n");
	}
	
	bstop(bfichier); 
	//AfficherArbre(ArbreHuffman);
}
//--------------------------------------------------------
void AfficherTexteEncode(FILE *nomf){
	
	int b;
	
	BFILE *bfichier = bstart(nomf, "r");
	
	do{
	
		b = bitread(bfichier);
		printf("%d", b);
		
	}while(!beof(bfichier));
	
	bstop(bfichier);
}
//--------------------------------------------------------
int main(int argc, char * argv[]) {
  
  fichier_encode = fopen(argv[1], "r");

  //AfficherTexteEncode(fichier_encode);
	
  Decoder();

  fclose(fichier_encode);
  
  return 0;
}
