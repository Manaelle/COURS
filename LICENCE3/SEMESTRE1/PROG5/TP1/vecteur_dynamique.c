#include "vecteur_dynamique.h"
#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------
vecteur allouer_vecteur(int taille)
{		
		vecteur v = malloc(sizeof(struct donnees_vecteur));
  	v -> donnees = malloc(taille*sizeof(double));
		v -> taille = taille;
  	return v;
}
//------------------------------------------------------
void liberer_vecteur(vecteur v)
{
	free(v->donnees);
	free(v);
}
//------------------------------------------------------
int est_vecteur_invalide(vecteur v)
{
  return ((v==NULL)||(v->donnees==NULL)?1:0);
}
//------------------------------------------------------
double *acces_vecteur(vecteur v, int i)
{
  
	if ((i<0) || (est_vecteur_invalide(v))) {return NULL;}
	else if (i< (v->taille)) { return (double *)(v->donnees+i); }
	else {
			v -> taille = i+1;
			return (realloc(v->donnees, (i+1)*sizeof(double))==NULL?NULL:v->donnees+i);
			}
}
//------------------------------------------------------
int taille_vecteur(vecteur v)
{
  return v->taille;
}
