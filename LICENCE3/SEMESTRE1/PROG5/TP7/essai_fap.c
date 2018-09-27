#include "fap.h"
#include <stdio.h>
#include <stdlib.h>

#define TAILLE_BUFFER 128

void aide()
{
  fprintf(stderr,"Aide :\n");
  fprintf(stderr,"Saisir l'une des commandes suivantes\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"i nombre priorite  :   inserer un nombre avec sa priorite\n");
  fprintf(stderr,"e                  :   extraire le nombre de priorite maximale\n");
  fprintf(stderr,"v                  :   teste si la fap est vide\n");
  fprintf(stderr,"h                  :   afficher cette aide\n");
  fprintf(stderr,"q                  :   quitter ce programme\n");
}

//----------------------------------

int tri_int(int a, int b)
{
  printf("Comparaison 1\n");
  return a < b;
}

//----------------------------------------------

int main()
{
  char buffer[TAILLE_BUFFER];
  char commande;
  int nombre,priorite;
  void *nb;
  fap f;

  f = creer_fap_vide(tri_int);
  nb = malloc(sizeof(int *));
  
  aide();
  while (1)
    {
      commande = getchar();
      switch (commande)
        {
        case 'i':
          scanf ("%d",&nombre);
          scanf ("%d",&priorite);
          f = inserer(f,(void*)&nombre, priorite);
          printf("(%d,%d) a ete insere\n",nombre,priorite);
          break;
        case 'e':
          if (!est_fap_vide(f))
            {
              f = extraire(f,nb,&priorite);
              printf("(%d,%d) a ete extrait\n",**(int**)nb, priorite);
            }
          else
              printf("La fap est vide !\n");
          break;
        case 'v':
          printf("Est_fap_vide a retourne %d\n",est_fap_vide(f));
          break;
        case 'h':
          aide();
          break;
        case 'q':
          detruire_fap(f);
          exit(0);
        default:
          fprintf(stderr,"Commande inconnue !\n");
        }
      /* vide ce qu'il reste de la ligne dans le buffer d'entree */
      fgets(buffer,TAILLE_BUFFER,stdin);
    }
  detruire_fap(f);
  return 0;
}
