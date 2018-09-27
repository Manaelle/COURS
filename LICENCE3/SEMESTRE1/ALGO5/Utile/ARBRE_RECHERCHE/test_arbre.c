#include <stdlib.h>
#include <stdio.h>
#include "arbre_recherche.h"
#include "fap.h"

int main(){

	int x;
	char choix;
	arbre B;

	B = ArbreVide();
	printf("Taper i pour insérer, s pour supprimer, a pour afficher, et q pour quitter :\n");
	scanf(" %c", &choix);

	while (choix != 'q') {

		switch(choix){

		case 'i' :
			printf("Insertion :\n");
			scanf(" %d", &x);
			B = Inserer(B, x);
			break;

		case 's' :
			printf("Suppression :\n");
			scanf(" %d", &x);
			B = Supprimer(B, x);
			break;

		case 'a' : 
			printf("Affichage :\n");
			Afficher(B);
			printf("\n");
			break;
		}

		printf("Ensuite ?\n");
		scanf(" %c", &choix);
	}
		
	return 0;
}