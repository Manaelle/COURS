#ifndef DEF_FAP
#define DEF_FAP

struct maillon;

typedef struct maillon *fap;

struct maillon {
void *Elem;
int Priorite;
fap Suivant;
};

fap Initialiser ();

int  EstFileVide (fap F);

int  EstPleine (fap F, int nbmax);

fap InsererFile (fap F, void *x, int p);

fap ExtraireFile (fap F, void **x, int *p);

void Detruire(fap F);

#endif
