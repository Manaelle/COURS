#ifndef DEF_ARBRE
#define DEF_ARBRE

struct noeud;

typedef struct noeud *arbre;

struct noeud {
int Elem;
arbre Gauche;
arbre Droit;
};

arbre ArbreVide();

int EstVide(arbre A);

int EstFeuille(arbre A);

arbre Inserer(arbre A, int E);

void AfficherPr(arbre A);

void Afficher(arbre A);

arbre Rechercher(arbre A, int E);

int Minimum(arbre A);

arbre Supprimer(arbre A, int E);

arbre SupprimerRacine(arbre T);

arbre SupprMin(arbre T);

#endif
