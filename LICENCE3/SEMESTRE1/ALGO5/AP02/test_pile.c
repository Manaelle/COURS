#include <stdlib.h>
#include <stdio.h>
#include "pile.h"

int main()
{
	pile D;
	char x ;
	
	D = PileVide();
	printf("Insérer un char :\n");

	do
	{
		 	scanf(" %c", &x);
		
		if (x == '1') {
			D = Depiler(D); 
			printf("Depiler : %c\n", Sommet(D));
			}
		else {
			D = Empiler(D, x);
			printf("Empiler : %c\n", Sommet(D));
		}

	} while (x != '0');

	printf("Fin\n");
return 0;
}
