#ifndef BIT_FILE
#define BIT_FILE

struct BFILE {
FILE *bfichier; //le fichier à lire ou à écrire
int buffer[8]; //l'octet à lire
int i; //à quel niveau on est dans l'octet
char mode; //le mode (w : écriture ou r: lecture)
};

typedef struct bfile *BFILE;

/*
   bstart
   description : ouvre un acces bit a bit en lecture (mode "r") ou bien en
                 ecriture (mode "w") au fichier dont le descripteur est passe
                 en parametre (le fichier doit avoir ete prealablement ouvert).
                 Pour conserver la coherence des donnees, aucun acces non bit a
                 bit au même descripteur ne doit etre fait jusqu'au prochain
                 bstop.
   parametres : descripteur du fichier, mode d'ouverture de l'acces
   valeur de retour : pointeur vers une structure BFILE allouee par bstart
                      ou NULL si une erreur est survenue
   effets de bord : aucun (outre l'allocation)
*/
BFILE *bstart(FILE *fichier, const char *mode);

/*
   bstop
   description : ferme un acces bit a bit a un fichier prealablement ouvert par
                 bstart (termine les E/S en attente et libere la structure
                 BFILE).
   parametres : pointeur vers une structure BFILE renvoyee par bstart
   valeur de retour : 0 si aucune erreur n'est survenue
   effets de bord : ecrit potentiellement dans le fichier
*/
int bstop(BFILE *fichier);

/*
   bitread
   description : lit un bit dans un fichier sur lequel un acces bit a bit en
                 lecture a ete prealablement ouvert a l'aide de bstart.
   parametres : pointeur vers une structure BFILE renvoyee par bstart
   valeur de retour : bit lu ou -1 si une erreur s'est produite ou si la
                      fin de fichier a ete atteinte
   effets de bord : la valeur de retour depend du contenu du fichier
                    lit potentiellement dans le fichier
*/
int bitread(BFILE *fichier);

/*
   bitwrite
   description : ecrit un bit dans un fichier sur lequel un acces bit a bit en 
                 ecriture a ete prealablement ouvert a l'aide de bstart.
   parametres : pointeur vers une structure BFILE renvoyee par bstart, bit a
                ecrire
   valeur de retour : -1 si une erreur s'est produite
   effets de bord : ecrit potentiellement dans le fichier
*/
int bitwrite(BFILE *fichier, int bit);

/*
   beof
   description : retourne 1 si un acces en lecture prealable a echoue pour
                 cause d'atteinte de la fin d'une sequence de bits (fin de
                 fichier ou fin de sequence codee dans le fichier), 0 sinon.
                 Le fichier doit avoir ete prealablement ouvert a l'aide de
                 bstart.
   parametres : pointeur vers une structure BFILE renvoyee par bstart
   valeur de retour : 1 ou 0
   effets de bord : aucun.
*/
int beof(BFILE *fichier);

#endif
