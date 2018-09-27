#include <stdlib.h>
#include <stdio.h>
#include "fap.h"

/* implémentation des primitives de la fap d'Attente à Priorités */


/* --------------------------------------------------
 * Initialiser
 * résultat : une FAP vide.
 * description : retourne la fap F, vide
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
fap Initialiser () {
  return NULL;
}

/* --------------------------------------------------
 * EstFileVide
 * donnée : une fap F
 * résultat : un booléen
 * description : vaut vrai ssi F est vide
 ----------------------------------------------------- */
int  EstVide (fap F) {
	return (F==NULL);
}

/* --------------------------------------------------
 * Inserer
 * données : un pid_t "pid",  un entier pr
 * donnée-résultat : une fap F
 * description :  insère le nouveau pid de priorité  pr
 * 			dans la fap F
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
fap Inserer (fap F, pid_t pid, int pr) {
  
  /*N est ne nouveau, C est le courant, et P est le précédent*/
  fap N;

  /* nouveau maillon */
  N = (fap)malloc(sizeof(struct maillon));
  N->Elem = pid;
  N->Priorite = pr;

  /* insertion en tete */
    N->Suivant = F;
    F = N;

  return F;
} 


/* --------------------------------------------------
 * Extraire
 * donnée-résultat : une fap F
 * résultats : un pid_t pid, un entier pr
 * description :  supprime de la fap F une requête 
 * 		de priorite maximale pr, parmi celles présentes dans F. 	
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
fap Extraire (fap F, pid_t *x) { 
  
  /*N est ne nouveau, C est le courant, et P est le précédent*/
  fap C, P;
  
  P = F;
  C = F;
  while ((C != NULL)&&(*x != C->Elem ))
    {
		P = C;
        C = C->Suivant;
	}
   

  /* Suppression du maillon de pid_t x */
	if (*x == C->Elem)
	{
		if (C==F){ 
			F = C->Suivant; 
			
		}
		else { 
			P->Suivant = C->Suivant; 
		}
		free(C);
	}
	else 
	{
		printf("ERREUR extraire : L'élément est absent !\n");
	}

  return F;
}

/* --------------------------------------------------
 * Afficher
 * donnée : une fap F
 * description : Affiche la file F
 ----------------------------------------------------- */
void  Afficher (fap F) {
	
	fap C;
	
	if (EstVide(F)) {printf("Vide\n");}
	
	C = F;
	while ((C != NULL))
        {
          printf("[ %d , %d ]  ", C->Elem, C->Priorite);
		  C = C->Suivant;
		  
        }		
	printf("\n");
}
/* --------------------------------------------------
 * Detruire
 * donnée-résultat : une fap F
 * description :  Libère complètement F.
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
void Detruire(fap F)
{	
	fap C;
	
	while (F != NULL)
	{
		C = F;
		F = F->Suivant;
		free(C);
	}
	free(F);
}

