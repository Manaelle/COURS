
/*
tri_insertion
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void tri_insertion(int t[], int n);

/*
tri_rapide
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void echanger(int t[], int i, int j);
int partition(int t[], int g, int d, int pivot, int *f);
int tri_segmentation(int t[], int g, int d);

/*  
lancer_mesures
Donnees : aucune
Resultat : lit les parametres au clavier, et effectue les mesures par
initialisations et tris successifs d'un tableau
*/
void lancer_mesures();

