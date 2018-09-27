#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "fap.h"
#define MAXX 50
# define MAXP 5

int main() {

  fap F;
  int nb_evenements ;
  int a, b, i;

  /* initialisation du generateur aleatoire */
  srand(time(NULL)) ;

  /* lecture des parametres de la simulation */
  printf("\n nombre d'evenements a generer : ") ;
  scanf("%d", &nb_evenements) ;

  /* simulation */
  F = Initialiser(F) ;

  for (i=0 ; i<nb_evenements ; i++) {
	  a = rand()%MAXX +1;
	  b = rand()%MAXP +1;
    F = Inserer(F, a, b);
  }
  printf("\n La file d'attente : ") ;
   Afficher(F);
   
   printf("\n Extraction : ") ;
   while(!EstVide(F)){
	   F = Extraire(F, &a, &b);
   }
   Afficher(F);
	
  Detruire(F);
  return 0 ;

}
