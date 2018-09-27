#ifndef DEF_FAP
#define DEF_FAP

struct maillon;

typedef struct maillon *fap;

struct maillon {
int Elem;
int Priorite;
fap Suivant;
};

fap Initialiser ();

int  EstVide (fap F);

int  EstPleine (fap F, int nbmax);

void  Afficher (fap F);

fap Inserer (fap F, int x, int p);

fap Extraire (fap F, int *x, int *p);

void Detruire(fap F);

#endif
