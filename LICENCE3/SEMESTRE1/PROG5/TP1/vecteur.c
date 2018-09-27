#include "vecteur.h"
#include <stdlib.h>
#include <stdio.h>

// int * t = (int*)malloc(32*sizeof(int)
vecteur allouer_vecteur(int taille)
{
  	double *d = (double*)malloc(taille*sizeof (double));
		vecteur v = {taille, d};
  	return v;
}
//------------------------------------------------------
void liberer_vecteur(vecteur v)
{
	free(v.donnees);
}
//------------------------------------------------------
int est_vecteur_invalide(vecteur v)
{
  return (v.donnees==NULL?1:0);
}
//------------------------------------------------------
double *acces_vecteur(vecteur v, int i)
{
  return (v.taille<i?NULL:v.donnees+i);
}
//------------------------------------------------------
int taille_vecteur(vecteur v)
{
  return v.taille;
}
//------------------------------------------------------
