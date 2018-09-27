#ifndef DEF_PILE
#define DEF_PILE
#define NMAX 500

typedef struct tab_pile {
  int t; 				/*indice de la tete de pile*/
  char tab[NMAX];		/*pile en question*/
}pile;

pile PileVide();

pile Empiler(pile P, char c);

int EstVide(pile P);

char Sommet(pile P);

pile Depiler(pile P);

#endif
