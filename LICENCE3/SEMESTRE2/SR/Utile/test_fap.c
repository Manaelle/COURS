
#include <stdlib.h>
#include <stdio.h>
#include "fap.h"


int main() {

  fap F;
  char saisie[2]; // Saisie au clavier
  int p; // Élément, priorité
  pid_t j;
  
  F = Initialiser();

  printf("Type d'évènement ['i' pour Insérer, 'e' pour Extraire, 'q' pour quitter] : ");
  scanf("%s",saisie);

  while(saisie[0] != 'q') {
	  
    switch (saisie[0]) {
    case 'i':
		printf("Élément à insérer : ");
		scanf("%d", &j);
		printf("Priorité de l'élément : ");
		scanf("%d",&p);
		F = Inserer(F,j,p);
		Afficher(F);
		break;
		
    case 'e':
      if (EstVide(F)) {
		printf("Erreur : la fap est vide !\n");
      } 
	  else {
		scanf("%d", &j);
		F = Extraire(F,&j);
		printf("Élément extrait : %d, de priorité %d\n",j,p);
		Afficher(F);
      };
      break;
    }
    printf("Type d'évènement ['i' pour insérer, 'e' pour ExtraireFile, 'q' pour quitter] : ");
    scanf("%s",saisie);
  }
  Detruire(F);
  return 0 ;

}
