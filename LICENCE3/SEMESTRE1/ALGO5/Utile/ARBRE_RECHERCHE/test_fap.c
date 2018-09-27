
#include <stdlib.h>
#include <stdio.h>
#include "fap.h"
#define NMAX 2

void  Afficher (fap F) {
	
	fap C;
	
	if (EstFileVide(F)) {printf("Vide");}
	
	C = F;
	while ((C != NULL))
        {
          printf("[ %d , %d ] ", (int)*(C->Elem), C->Priorite);
		  C = C->Suivant;
		  
        }		
	printf("\n");
}

int main() {

  fap F;
  char saisie[2]; // Saisie au clavier
  int x, p; // Élément, priorité
  void *d;
  
  F = Initialiser();

  printf("Type d'évènement ['i' pour insérer, 'e' pour ExtraireFile, 'q' pour quitter] : ");
  scanf("%s",saisie);

  while(saisie[0] != 'q') {
	d = (int)malloc(sizeof(int));
    switch (saisie[0]) {
    case 'i':
      if (EstPleine(F, NMAX)) {
		printf("Erreur : la fap est pleine !\n");
      } 
	  else {
		printf("Élément à insérer : ");
		scanf("%d",&x);
		*d = x;
		printf("Priorité de l'élément : ");
		scanf("%d",&p);
		F = InsererFile(F,d,p);
		Afficher(F);
      };
      break;
    case 'e':
      if (EstFileVide(F)) {
		printf("Erreur : la fap est vide !\n");
      } 
	  else {
		F = ExtraireFile(F,&x,&p);
		printf("Élément extrait : %d, de priorité %d\n",x,p);
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
