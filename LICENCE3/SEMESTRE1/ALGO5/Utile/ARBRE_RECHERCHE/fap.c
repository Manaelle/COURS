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
int  EstFileVide (fap F) {
	return (F==NULL);
}

/* --------------------------------------------------
 * EstPleine
 * donnée : une fap F, un entier nbmax
 * résultat : un booléen
 * description : Compte le nombre d'éléments dans la FAP. vaut vrai ssi F est pleine, cad que le nombre d'élément = nbmax
 ----------------------------------------------------- */
int  EstPleine (fap F, int nbmax) {
	int nb =0;
	fap C;
	
	C = F;
	while ((C != NULL))
        {
          C = C->Suivant;
		  nb++;
        }		
	
	printf("Il y a %d éléments dans la file. \n", nb);
	return (nb>=nbmax) ;
}


/* --------------------------------------------------
 * InsererFile
 * données : un entier x,  un entier p
 * donnée-résultat : une fap F
 * description :  insère la nouvelle requête x de priorité  p
 * 			dans la fap F
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
fap InsererFile (fap F, void *x, int pr) {
  
  /*N est ne nouveau, C est le courant, et P est le précédent*/
  fap N, C, P;

  /* nouveau maillon */
  N = (fap)malloc(sizeof(struct maillon));
  N->Elem = x;
  N->Priorite = pr;

  /* insertion en tete */
  if ((F == NULL) || (pr > F->Priorite))
    {
      N->Suivant = F;
      F = N;
    }

  /* recherche de la bonne position et insertion */
  else
    {
      P = F;
      C = F->Suivant;
      while ((C != NULL)&&( pr <= C->Priorite))
        {
          P = C;
          C = C->Suivant;
        }
        
      P->Suivant = N;
      N->Suivant = C;
    }
  return F;
} 


/* --------------------------------------------------
 * ExtraireFile
 * donnée-résultat : une fap F
 * résultats : un entier x, un entier p
 * description :  supprime de la fap F une requête 
 * 		de priorite maximale p, parmi celles présentes dans F. 	
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
fap ExtraireFile (fap F, void *x, int *pr) {
  
  fap C;

  /* ExtraireFile le premier element si la fap n'est pas vide */
  if (F != NULL)
    {
      C = F;
      x = C->Elem;
      *pr = C->Priorite;
      F = C->Suivant;
      free(C);
    }
  return F;
}

/* --------------------------------------------------
 * Detruire
 * donnée-résultat : une fap F
 * description :  Libère complètement F.
 * effet de bord : F est modifiée
 ----------------------------------------------------- */
void Detruire(fap F)
{	
	int p;
	void *x;
	
	while (F != NULL)
	{
		F = ExtraireFile(F,&x, &p);
		free(x);
	}
	free(F);
}

