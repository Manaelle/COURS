#ifndef DEF_FAP
#define DEF_FAP

struct maillon;

typedef struct maillon *fap;

struct maillon {
pid_t Elem;
int Priorite;
fap Suivant;
};

fap Initialiser ();

int  EstVide (fap F);

fap Inserer (fap F, pid_t pid, int pr);

fap Extraire (fap F, pid_t *x);

void  Afficher (fap F);

void Detruire(fap F);

#endif
