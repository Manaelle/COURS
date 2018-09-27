#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define NMAX 100

/*--------------------------------------*/
//TYPES
typedef struct {
	int n;
	int L[NMAX];
} liste;

/*--------------------------------------*/
//PROTOTYPES
liste CreerListe();
void AfficherListe(liste E);
liste Ajouter(liste E, int X);
liste Supprimer(liste E);
void EnumerationSimple(liste E, int i, liste Y);

/*--------------------------------------*/
//FONCTIONS
liste CreerListe(){
	
	const char delim[1] = " ";
	char *word;
	char *save;
	int i=0;
	char cmd[NMAX];
	liste E;
	
	
	E.n = 0;
	fgets(cmd, sizeof(cmd), stdin);
   
   //Pour chaque mot, le ranger
   save = cmd; 
   while ((word = strtok_r(save, " ", &save))){
			E.L[i] = atoi(word);
			E.n++;
			i++;
   }
   
   return E;
	
}
/*----*/
void AfficherListe(liste E){
	
	int i;
	
	printf("[ ");
	for(i=0;i<E.n; i++){
		printf("%d ", E.L[i]);
	}
	printf("]\n");
	
}
/*----*/
liste Ajouter(liste E, int X){
	
	E.L[E.n] = X;
	E.n++;
	
	return E;
}
/*----*/
liste Supprimer(liste E){
	//supp du dernier élément
	E.n--;
	
	return E;
}
/*----*/
void EnumerationSimple(liste E, int i, liste Y){
	
	if (Y.n == 3){
		AfficherListe(Y);
	}
	else if (i==0){
		return;
	}
	else{	
		EnumerationSimple(E, i-1, Y);
		Y = Ajouter(Y, E.L[i-1]);
		E = Supprimer(E);
		EnumerationSimple(E, i-1, Y);
		
	}	
}
/*--------------------------------------*/
//MAIN
int main(){

	liste M, Y;
	Y.n = 0;
	M = CreerListe();
	
	printf("N = %d\n", M.n);
	AfficherListe(M);
	printf("---------\n");
	
	//M = Ajouter(M, 13);
	//AfficherListe(M);
	
	EnumerationSimple(M, M.n, Y);
	
  return 0;
}


