#include "fap.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


//----------------------------------
fap creer_fap_vide(int (*comparer)(int, int))
{
  fap resultat;
  
	resultat.comparer = comparer;
	resultat.tete = NULL;
  return resultat;
}

//----------------------------------
fap inserer(fap f, void *element, int priorite){

  struct maillon *nouveau, *courant, *precedent;
  //void * pelem;
	
	//Création d'un maillon + remplissage de celui-ci
  nouveau = (struct maillon *) malloc(sizeof(struct maillon));
  //pelem = malloc(sizeof(int *));
  
  nouveau->element = element;
  nouveau->priorite = priorite;
  printf("insérer debbug : %d %d \n", *((int*)nouveau->element), nouveau->priorite );
  //Placement dans la file selon sa priorité
  if ((f.tete == NULL) || f.comparer(priorite, f.tete->priorite)) //On utilise la fonction de comparaison ici !
    {
      nouveau->prochain = f.tete;
      f.tete = nouveau;
    }
  else
    {
      precedent = f.tete;
      courant = precedent->prochain;
      
      while ((courant != NULL) && !f.comparer(priorite, courant->priorite)) //...Et ici !
        {
          precedent = courant;
          courant = courant->prochain;
        }
      precedent->prochain = nouveau;
      nouveau->prochain = courant;
    }
  return f;
}
 
//---------------------------------- 
// EXTRAIRE : la fap étant déjà trié par priorité, il suffit d'extraire la tête
fap extraire(fap f, void **element, int *priorite)
{
  struct maillon *courant;
	
	
  if (!est_fap_vide(f))
    {
      courant = f.tete;
      *element = courant->element;
      *priorite = courant->priorite;
	  printf("extraire debbug : %d %d %d\n", **((int**)element),*(int*)courant->element, *priorite);
      f.tete = courant->prochain;
      free(courant);
    }
  return f;
}

//----------------------------------
int est_fap_vide(fap f)
{
  return f.tete == NULL;
}

//----------------------------------
void detruire_fap(fap f)
{
  void *element = NULL;
  void *priorite = NULL;

  while (!est_fap_vide(f))
      f = extraire(f,element,priorite);
}
