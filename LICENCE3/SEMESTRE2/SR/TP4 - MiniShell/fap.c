#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
fap Inserer (fap F, pid_t pid, int pr, char *cj) {
  
  /*N est ne nouveau, C est le courant, et P est le précédent*/
  fap N;

  /* nouveau maillon */
  N = (fap)malloc(sizeof(struct maillon));
  N->Elem = pid;
  N->Priorite = pr;
  N->Cmdjob = malloc(strlen(cj)*sizeof(char));
  strcpy(N->Cmdjob, cj);

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
		free(C->Cmdjob);
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
	printf("-----------------------------------------------------\n");
	printf("Liste des jobs : \n");
	if (EstVide(F)) {printf("Aucun jobs actif.\n");}
	else {
		printf("%-5.5s \t", "[PID]");
		printf("%-8.8s \t", "[N°Job]");
		printf("%-15.15s\n", "[CMD]");
		}
	
	C = F;
	while ((C != NULL))
        {
			printf("%-5.5d \t", C->Elem);
			printf("%-8.8d \t", C->Priorite);
			printf("%-15.15s\n", C->Cmdjob);
			C = C->Suivant;
		  
        }		
	printf("\n");
	printf("-----------------------------------------------------\n");
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
		free(C->Cmdjob);
		free(C);
	}
	free(F);
}

