#ifndef DEF_TP1
#define DEF_TP1

//----------- DONNEES ---------------
typedef struct processus{
	int ID; //identifiant du processus (0 est le processus patriarche)
	pid_t pid; //pid du processus
} processus;

//---------- FONCTIONS --------------
int getID(pid_t pid, int n, processus P[]);

//CREER_FILS : prend en paramètre ne nombre n de processus à créer,
//et construit un circuit de n processus reliés entre eux par n-1 tubes. 
void creer_fils(int n, processus P[], int tubes[][2]);

int main(int argc, char *argv[]);

#endif
