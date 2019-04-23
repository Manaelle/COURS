#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//-----------------------------------------
//--------------- DONNEES -----------------
//-----------------------------------------
typedef struct vecteur{
	int t[];
	int nb_elem;
}vecteur;



//-----------------------------------------
//--------------- FONCTIONS ---------------
//-----------------------------------------
