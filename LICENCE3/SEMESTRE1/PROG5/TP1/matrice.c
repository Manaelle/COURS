#include "matrice.h"
#include <stdlib.h>

//------------------------------------------------------
matrice allouer_matrice(int l, int c) 
{	
	int i;
	double **donnees = (double**)malloc(l*sizeof (double*));
	for (i=0;i<l;i++)
	{
		donnees[i] = (double*)malloc(c*sizeof(double));  
	}
  matrice m = {l, c, donnees};
  return m;
}
//------------------------------------------------------
void liberer_matrice(matrice m)
{
	int i;
	for (i=0;i<m.l;i++)
	{
		free(*(m.donnees+i));
	}
	free(m.donnees);
}
//------------------------------------------------------
int est_matrice_invalide(matrice m)
{
  if (m.donnees==NULL)return 1;
  int i;
  for (i=0;i<m.l;i++)
   {
  if (m.donnees[i]==NULL)return 1;
   }
  return 0;
}
//------------------------------------------------------
double *acces_matrice(matrice m, int i, int j)
{
  return (i<m.l && j<m.c ?  m.donnees[i]+j: NULL);
}
//------------------------------------------------------
int nb_lignes_matrice(matrice m)
{
  return m.l;
}
//------------------------------------------------------
int nb_colonnes_matrice(matrice m)
{
  return m.c;
}
