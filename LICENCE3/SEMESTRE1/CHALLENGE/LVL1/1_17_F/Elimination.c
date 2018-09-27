#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NMAX 100

/*--------------------------------------*/
//TYPES
typedef struct {
	int n;
	int T[NMAX];
} tab;

/*--------------------------------------*/
//PROTOTYPES
tab Suppr(tab G, int k);
void Affichage(tab G);
tab Calcul(tab G);

/*--------------------------------------*/
//FONCTIONS
tab Suppr(tab G, int k){
	//suppression de l'élément T[k] + décalage
	int i;
	if (k >= G.n || k < 0){ //printf("ERREUR : Supp invalide car k incorrect.\n");
					return G;} 
	
	if (G.T[k] == 0){ //printf("ERREUR : Supp invalide, ne pas suppr le zéro de fin !\n");
					return G;} 
					
	for(i=k; i < G.n; i++){
		G.T[i] = G.T[i+1];
	}
	G.n--;
	
	return G;
}
/*----*/
void Affichage(tab G){
	//Affichage de T
	int i;
	
	//printf("N = %d \n", G.n);
	for(i=0; i < G.n- 1; i++){
		
		printf("	%d", G.T[i]);
	}
	printf("	0");
	
	return;
}
/*----*/
tab Calcul(tab G){
//Vérifie si la somme des prédessesseurs de G.T[i] est égal à celui-ci. Si oui, effectue la suppr de G.T[i]
	int i,j,R;

		for(i = G.n-1; i >= 0; i--){
			R = 0;
			for(j=i-1; j >= 0 ; j--){
				R = G.T[j] + R;
			}
			
			//printf("R = %d \n", R);
			
			if (R == G.T[i]) {
				G = Suppr(G, i);
				//printf("R SUPPR = %d \n", R);
				return G;
			}
		}
	return G;
}
/*----*/
int EstEgal(tab G, tab H){
//Renvoie 1 si G et H sont égaux, 0 sinon

	int i;
	
	if (G.n != H.n){return 0;} 
	else {
	
		for(i=0; i < G.n ; i++){
			if (H.T[i] != G.T[i]) {
				return 0;
			}
		}
	}
	return 1;
}
/*----*/
tab Sauvegarde(tab G){

	int i;
	tab H;
	
	H.n = G.n;

	for(i=0; i < G.n ; i++){
		H.T[i] = G.T[i];
	}
	
	return H;
}
/*----*/
tab Elimination(tab G){
//Effectue les calculs jusqu'à qu'il n'y aie plus rien à supprimer.
	tab S;
	
		do{
			S = Sauvegarde(G);
			G = Calcul(G);
			//Affichage(G);
			
		}while ( !EstEgal(G,S) );
	
	return G;
}
/*----*/
tab Split_line(char cmd[NMAX], tab G){
	
	const char delim[1] = " ";
	char *word;
	int i=0;
	
	word = strtok(cmd, delim);
   
   //Pour chaque mot, le ranger dans G.T[i] 
   while( word != NULL && i<NMAX ){

		G.T[i] = atoi(word);
		G.n++;
		i++;
		word = strtok(NULL, delim);
   }
   
   return G;
	
}
/*--------------------------------------*/
//MAIN
int main() {

	tab E;
	char c[100];
	
	//Initialisation
	E.n = 0;
	fgets(c, sizeof(c), stdin);
	E = Split_line(c,E);
	
	//Affichage(E);
	//printf("----------\n");
	
	//Calcul
	E = Elimination(E);
	Affichage(E);

  return 0 ;
}



