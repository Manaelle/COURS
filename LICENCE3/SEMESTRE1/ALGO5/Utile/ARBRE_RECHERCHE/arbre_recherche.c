#include <stdlib.h>
#include <stdio.h>
#include "arbre_recherche.h"
#include "fap.h"

arbre ArbreVide(){

	return NULL;
}

arbre N(arbre G, int E, arbre D){
//créer un noeud
	arbre A;
	
	A = ArbreVide();
	A = (arbre)malloc(sizeof(struct noeud));
	A -> Gauche = G;
	A -> Droit = D;
	A -> Elem = E;
	return A;
}

int EstVide(arbre A){

	return (A == NULL);
}

int EstFeuille(arbre A){

	if (!EstVide(A)){
		return (A -> Gauche == NULL) && (A-> Droit == NULL);
		}
	
	return 0;
}

arbre Inserer(arbre A, int E){
//insère au bon endroit, le noeud d'élément E	
	if (EstVide(A)) {
		return N( ArbreVide(), E, ArbreVide() );
	}
	else if ( E <= (A -> Elem) ) {
		A -> Gauche = Inserer( (A -> Gauche), E);
		return A;
	}	
	else {
		A -> Droit = Inserer( (A -> Droit), E);
		return A;
	}

}

void AfficherPr(arbre A){
	
	if (!EstVide(A)){
		AfficherPr(A -> Gauche);
		printf("[ %d ]", A -> Elem);
		AfficherPr(A -> Droit);
	}
}


void Afficher(arbre A){
//Affichage par parcours en largeur
	fap F;
	
	arbre N;
	int pr = 1;
	

	F = Initialiser();
	N = ArbreVide();
	F = InsererFile(F, &A, pr);
	printf("truc2");
	while (!EstFileVide(F)){
		
			F = ExtraireFile(F, N, &pr);
			printf("[ %d ]", N -> Elem);
			if (!EstVide(N)){
				printf("truc3");
				if(N -> Gauche != NULL) { F = InsererFile(F, &(N -> Gauche), pr); }
				if(N -> Droit != NULL) { F = InsererFile(F, &(N -> Droit), pr); }
				printf("[ %d ]", N -> Elem);
			}
		printf("\n");
	}
	
}

arbre Rechercher(arbre A, int E){
// Renvoie le noeud d'élément E	
	if (EstVide(A)) {
		return ArbreVide();
	}
	else if ( E == (A -> Elem) ) {
		return A;
	}
	else if ( E < (A -> Elem) ) {
		return Rechercher((A -> Gauche), E);
	}	
	else {
		return Rechercher((A -> Droit), E);
	}

}

int Minimum(arbre A){
	
	if ( EstVide(A-> Gauche) ){
		return A -> Elem;
	}
	else {
		return Minimum(A-> Gauche);
	}

}

arbre Supprimer(arbre A, int E){
	
	if (EstVide(A)) {
		return ArbreVide();
	}
	else if ( E == (A -> Elem) ) {
		SupprimerRacine(A);
	}
	else if ( E < (A -> Elem) ) {
		return Supprimer((A -> Gauche), E);
	}	
	else {
		return Supprimer((A -> Droit), E);
	}
	return ArbreVide();

}

arbre SupprimerRacine(arbre T){
	
	int M;
	
	if (EstVide(T -> Gauche)){
		return (T -> Droit);
	}
	else if (EstVide(T -> Droit)){
		return (T -> Gauche);
	}
	else {
		M = Minimum(T -> Droit);
		return N(T -> Gauche ,M, SupprMin(T -> Droit));				
	}
}

arbre SupprMin(arbre T){
	
	if ( EstVide(T-> Gauche) ){
		return (T -> Droit);
	}
	else if ( EstFeuille(T-> Gauche) ){
		T -> Gauche = ArbreVide();
		return T;
	}
	else {		
		T -> Gauche = SupprMin(T-> Gauche);		
		return T;
	}

}