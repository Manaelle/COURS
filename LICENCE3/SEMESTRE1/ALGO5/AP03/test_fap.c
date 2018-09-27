
#include <stdlib.h>
#include <stdio.h>
#include "fap.h"
#define NMAX 2

int main() {

  fap F;
  char saisie[2]; // Saisie au clavier
  int x, p; // Élément, priorité
  
  F = Initialiser();

  printf("Type d'évènement ['i' pour insérer, 'e' pour extraire, 'q' pour quitter] : ");
  scanf("%s",saisie);

  while(saisie[0] != 'q') {
    switch (saisie[0]) {
    case 'i':
      if (EstPleine(F, NMAX)) {
		printf("Erreur : la fap est pleine !\n");
      } 
	  else {
		printf("Élément à insérer : ");
		scanf("%d",&x);
		printf("Priorité de l'élément : ");
		scanf("%d",&p);
		F = Inserer(F,x,p);
		Afficher(F);
      };
      break;
    case 'e':
      if (EstVide(F)) {
		printf("Erreur : la fap est vide !\n");
      } 
	  else {
		F = Extraire(F,&x,&p);
		printf("Élément extrait : %d, de priorité %d\n",x,p);
		Afficher(F);
      };
      break;
    }
    printf("Type d'évènement ['i' pour insérer, 'e' pour extraire, 'q' pour quitter] : ");
    scanf("%s",saisie);
  }
  Detruire(F);
  return 0 ;

}
