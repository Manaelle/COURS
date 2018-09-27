#include "matrice_lineaire.h"
#include <stdlib.h>

//------------------------------------------------------
matrice allouer_matrice(int l, int c)
{
  double * d = (double*)malloc(l*c*sizeof(double));
  matrice m = {l, c, d};
  return m;
}
//------------------------------------------------------
void liberer_matrice(matrice m)
{
  free (m.donnees);
}
//------------------------------------------------------
int est_matrice_invalide(matrice m)
{
  return (m.donnees==NULL?1:0);
}
//------------------------------------------------------
double *acces_matrice(matrice m, int i, int j)
{
  return m.donnees+i*m.c+j;
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
//------------------------------------------------------
