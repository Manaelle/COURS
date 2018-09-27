#include <stdlib.h>
#include <stdio.h>

#define NMAX 50000		/* peut etre modifie si necessaire */



/*
generation_aleatoire
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647] selon une loi uniforme.
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/

void generation_aleatoire(int t[], int n) {
  int i ;

  for (i=0 ; i<n ; i++) {
	  t[i] = rand() ;
  } ;
}

/*
generation_aleatoire_non_uniforme
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647].
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/
void generation_aleatoire_non_uniforme(int t[], int n) {
  int i;
  int x = 0;

  for (i=0; i < n; i++) {
    t[i] = x;
    x = x + ((rand() % 10) - 2);
  }
}
//-------------------------------------------------
/*
tri_insertion
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
int tri_insertion(int t[], int n) {
  int i,j;
  int Clef;
  int f=0; //compteur de comparaisons

  for(i=1; i < n; i++) {
    
    Clef = t[i];
    j = i - 1;

    /* Decalage des valeurs du tableau */
    while((j >= 0) && (t[j] > Clef)) {
      t[j+1] = t[j];    
      j = j - 1;
      
	  	f++;
    }
    
    if(j>=0){
    	f++;
    	
  	}

    
    /* insertion de la clef */
    t[j+1] = Clef;
  }
  //printf("Nombre de comparaisons : %d. \n", f);
  return f;
}
//-------------------------------------------------
/*
tri_segmentation
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
// Echanger : échanger les éléments des indices i et j : t[i] <=> t[j]
void echanger(int t[], int i, int j){
	int e;
	
	e = t[i];
	t[i] = t[j];
	t[j] = e;
}

// Partition
int partition(int t[], int g, int d, int pivot, int *f) {

	int i=g;

	while (i < d) {
	
		if (t[i+1] < pivot) {
			i++;
		}
		else {
			echanger(t, d, i+1);
			d--;
			*f = *f + 1;
		}		
	}
	
	echanger(t, g, i);
	*f = *f + 1;
	 
	
return i;
}
/*
tri_segmentation
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
int tri_segmentation(int t[], int g, int d) {

  int pivot, i, r;
  int *f;
  
  f = malloc(sizeof(int));
  *f = 0;

	if (g<d) {
	pivot = t[g];
	i = partition(t, g, d, pivot, f);
	tri_segmentation(t, g, i-1);
	tri_segmentation(t, i+1, d);
	}
	
	r = *f;
	free(f);
	return r;
}



//-------------------------------------------------
void lancer_mesures() {

  unsigned int germe ;
  int T[NMAX];
  int N, X, Y, i, j, fmoy=0, f=0, ftot=0;

  printf("Valeur du germe pour la fonction de tirage aleatoire ? ") ;
  //scanf("%d", &germe) ;
  germe = 5;
  srand(germe) ;

  do {
  	printf("Valeur de N ? ") ;
  	scanf("%d", &N) ;
  } while (N<1 || N > NMAX) ;
  
  X = 100;
	Y = 10;
	
	for(j=0; j<Y; j++){
		
		fmoy=0;
		f = 0;
		
		for(i=0; i<X; i++){
			
			generation_aleatoire(T, N) ; /* initialisation du tableau T */
			f = tri_insertion(T, N) + f ;
			//f = tri_segmentation(T, 0, N-1) + f ;	/* tri de T */
  	}
  	
  	fmoy = f/X;
		printf("Nombre de comparaisons moyenne sur %d exécutions : %d \n", X, fmoy);
		ftot = fmoy + ftot;
  	
	}
	
	ftot= ftot/Y;
	printf("Moyenne des fmoy sur %d exécutions : %d \n", Y, ftot);
	
	
  
}
