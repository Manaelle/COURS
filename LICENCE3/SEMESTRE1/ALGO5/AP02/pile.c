#include <stdlib.h>
#include <stdio.h>
#include "pile.h"

pile PileVide()
{
	pile P;
	
	P.t = 0;
	
	return P;
}

pile Empiler(pile P, char c)
{
	P.t = P.t + 1;
	P.tab[P.t] = c; 
	
	return P;
}

// renvoie 1 si la pile est vide, sinon 0
int EstVide(pile P)
{	

	if (P.t==0) {return 1;}
	else
		{return 0;}
}

char Sommet(pile P)
{
		if (EstVide(P)==1){printf("Erreur : Impossible de voir le sommet d'une pile vide." );
								exit(1);}

	return P.tab[P.t];
}

pile Depiler(pile P)
{
	if (EstVide(P)==1){printf("Erreur : Impossible de dépiler une pile vide." );
							exit(1);}
	P.t = P.t - 1;
	return P;
}


