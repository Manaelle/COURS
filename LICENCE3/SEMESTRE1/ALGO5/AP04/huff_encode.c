#include <stdio.h>
#include "fap.h"
#include "arbrebin.h"
#include "bfile.h"
#include "bonus.h"
#include "huffman_code.h"
#include <assert.h>
#define BONUS_1

FILE * fichier ;
FILE * fichier_encode;

int TableOcc[256] ;
fap file;
Arbre ArbreHuffman;

// Décommentez la ligne suivante si vous avez réalisé le bonus correspondant
// ATTENTION : les bonus ne seront pris en compte que si le reste fonctionne

struct code_char HuffmanCode[256];

//--------------------------------------------------------
void  ConstruireTableOcc () {

#ifdef BONUS_1

  int j, c;
  
  printf("----------------------------------\n");
  printf("INITIALISATION TableOcc : OK \n");
  //Initialisation de TableOcc
  for (j=0 ; j<256 ; j++) {
    TableOcc[j] = 0; 
      }
  
  //Remplissage
  c = fgetc(fichier);
  while (c != EOF) {
   	TableOcc[c]++;
    c = fgetc(fichier);
  }
  
#else
  bonus_ConstruireTableOcc(TableOcc, fichier);
#endif
  
  /*int i;
  for (i=0 ; i<256 ; i++) {
    if (TableOcc[i] != 0) { 
      printf("Occurences du caractere %c (code %d) : %d \n", i, i, TableOcc[i]) ; 
      }
  }*/

}
//--------------------------------------------------------
void InitHuffman() {

  file = creer_fap_vide();
  ArbreHuffman = ArbreVide();
  
}
//--------------------------------------------------------
Arbre ConstruireArbre() {

  int i, p1, p2;
  Arbre X, Y, Z;
  
  X = ArbreVide();
  Y = ArbreVide();
  Z = ArbreVide();
  
  printf("----------------------------------\n");
  printf("CONSTRUCTION DE L'ARBRE : \n");
  
  for (i=1 ; i<256 ; i++) {
				Z = NouveauNoeud(ArbreVide(), (unsigned char)i, ArbreVide());
				file = inserer(file, Z, TableOcc[i] );
  }
  
  printf("Construction des feuilles : OK \n");
  //ATTENTIONNNN GROS BUG ICI (corrigé :D)
  for (i=2 ; i<256 ; i++) {
  	
  	file = extraire(file, &X , &p1);
  	file = extraire(file, &Y , &p2);
  	Z = NouveauNoeud(X, '~', Y);
  	file = inserer(file, Z, p1+p2 );
  	
  }
  printf("Construction des noeuds : OK \n");
  return Z;
}

//--------------------------------------------------------
void ParcoursArbre(Arbre T, int lg, int bit[256]){
	
	int i;
	//Le fils gauche code un 1, le fils droit code un 0. (à prendre en compte dans le décodage)
	if (!EstVide(FilsDroit(T)) && !EstVide(FilsGauche(T)) ){
		bit[lg] = 1;
		lg = lg + 1;
		ParcoursArbre(FilsGauche(T), lg, bit);
		lg = lg - 1;
		bit[lg] = 0;
		lg = lg + 1;
		ParcoursArbre(FilsDroit(T), lg, bit);
		lg = lg - 2;
	}
	else {
		for(i = 0; i < lg; i++ ){
				HuffmanCode[Etiq(T)].code[i] =  bit[i] ;
				HuffmanCode[Etiq(T)].lg = lg;
				
		}
	}
}
//--------------------------------------------------------
void ConstruireCode(Arbre huff) {
  /* Construire la table de codage : remplir HuffmanCode[256] */
  int l = 0 ;
  int bitcode[256];
 
 ParcoursArbre(huff, l, bitcode);
 printf("Construction de la table de codage : OK \n");
   
}
//--------------------------------------------------------
void AfficherCodage(){
	
	int i,j;
	
	printf("----------------------------------\n");
	printf("Tableau de Codage (+ nombre d'occurence du symbole): \n");
	for(i=0;i<256;i++){
	
		if(TableOcc[i] != 0){
			printf("ASCII %d : (%d) ", i, TableOcc[i]);
			
			for(j=0;j<HuffmanCode[i].lg;j++){
				printf("%d", HuffmanCode[i].code[j]);
			}
			printf("\n");
		}
	}
}

//--------------------------------------------------------
void Encoder() {

  int c, i;
  
   printf("----------------------------------\n");
  printf("ENCODAGE : AJOUT DE L'ARBRE DE HUFFMAN\n");
  EcrireArbre(fichier_encode, ArbreHuffman);
  printf("Ecriture de l'arbre dans le fichier : OK \n");

 printf("\nENCODAGE : AJOUT DU FICHIER CODE\n");
  BFILE *bfile=bstart(fichier_encode, "w");
  c = fgetc(fichier);
  
  while (c != EOF) {
    for(i=0; i<HuffmanCode[c].lg; i++) {
      bitwrite(bfile, HuffmanCode[c].code[i]);
    }
    c = fgetc(fichier);
  }
  printf("Encodage du texte + écriture dans le fichier : OK \n");
  bstop(bfile);
  
  return;
}
//-------------------------------------------------------- 
int main (int argc, char *argv[]) {
  
  
  fichier = fopen (argv[1], "r") ;
   
  /* Construire la table d'occurences */
  ConstruireTableOcc () ;
  fclose(fichier);
  
  /* Initialiser la FAP */
  InitHuffman();
  
  /* Construire l'arbre d'Huffman */
  ArbreHuffman = ConstruireArbre();
  
  //AfficherArbre(ArbreHuffman);

  /* Construire la table de codage */
  ConstruireCode(ArbreHuffman);
  
  /* Encodage */
  fichier = fopen (argv[1], "r");
  fichier_encode = fopen(argv[2], "w+");

  Encoder();
  AfficherCodage();
  
  fclose(fichier_encode);
  fclose(fichier);
  
  return 0 ;
}
