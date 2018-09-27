#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define NMAX 100
#define NORD 0
#define SUD 1
#define EST 2
#define OUEST 3


/*--------------------------------------*/
//TYPES
typedef int direction;

typedef struct grille {
	int l, h;
	int Tab[NMAX][NMAX];
	direction dir;
} grille;

typedef struct boule {
	int x, y;
	int taille;
	boule Hist[NMAX];
} boule;


/*--------------------------------------*/
//PROTOTYPES
grille Init();
void Afficher(grille G);
int EstACote(boule A, boule B);
int Obstacle(grille G, boule A, boule B);
int EstLancable(grille G, boule A, boule B);
grille Lancee(grille G, boule A, boule B);
boule Chercher(grille G, boule A);
void AfficherHist(boule A);

/*--------------------------------------*/
//MAIN
int main(int argc, char *argv[]) {

	grille J;
	boule M,N;
		
	J = Init();
	Afficher(J);
	
	M.x = 1;
	M.y = 1;
	M = Chercher(J,M);
	AfficherHist(M);
	 
  return 0 ;

}

/*--------------------------------------*/
//FONCTIONS
grille Init(){
	/*Initialise le grillage à partir de l'entrée standard*/
	
	char buff[200];
	grille G;
	int i, j, x, a;
	
	G.l = 0;
	G.h = 0;
	G.dir = NORD;
	
	//lire l'entrée standard
	a = read(0, buff, 200);
	
	//Conversion en grille
	i = 0;
	j = 0;
	
	for(x=0; x <= a; x++){			
		if ( buff[x] == '\n' ) {
			j++;
			G.h++;
			G.l = i;
			i = 0;
		}
		else {
			G.Tab[i][j] = buff[x] - '0';
			i++; 
		}
	}
		

	return G;
}

/*----*/
void Afficher(grille G){
//Affichage de la grille.
	
	int i,j;
	
	printf("Dimension : %d x %d \n",G.l, G.h);
	printf("Dernier orientation enregistré : %d\n",G.dir);
	
	printf("Tableau :\n");
	for(j=0; j < G.h; j++){
		for(i=0; i < G.l; i++){
				printf("%d ",G.Tab[i][j]);
		}
		printf("\n");
	}

}

/*----*/
int EstACote(boule A, boule B){
// retourne 1 si A et B sont à coté, 0 sinon.

	
	if (	((B.x== A.x+1)&&(B.y== A.y)) //si B est à droite de A
					|| ((B.x== A.x-1)&&(B.y== A.y)) //si B est à gauche de A
					|| ((B.y== A.y +1)&&(B.x== A.x)) //si B est en bas de A
					|| ((B.y== A.y -1)&&(B.x== A.x)) ) { //si B est en haut de A
						
						return 1;
						
					}	
	else {return 0;}
	
}

/*----*/
int Obstacle(grille G, boule A, boule B){
// retourne le nombre de boules se situant entre A et B, ou -1 si erreur.
	int ecart = 0;
	int i;
	int nboules = 0;

	if ( EstLancable(G, A, B) ){
	
		// Si A est en bas de B
		if (A.x == B.x && A.y > B.y) {
			ecart = A.y - B.y - 1;
			for(i=1; i<=ecart ; i++){
				if (G.Tab[A.x][A.y - i] == 1) { nboules++; }		
			}					 
			return nboules;
		} 
		// Si A est en haut de B
		else if (A.x == B.x && A.y < B.y) {
			ecart = B.y - A.y - 1;
			for(i=1; i<=ecart ; i++){
				if (G.Tab[A.x][A.y + i] == 1) { nboules++; }		
			}	 
			return nboules;
		} 
		// si A est à droite de B
		else if (A.y == B.y && A.x > B.x) {
			ecart = A.x - B.x - 1;
			for(i=1; i<=ecart ; i++){
				if (G.Tab[A.x - i][A.y] == 1) { nboules++; }		
			}	
			return nboules;
		}
		//si A est à gauche de B
		else if (A.y == B.y && A.x < B.x) {
			ecart = B.x - A.x - 1;
			for(i=1; i<=ecart ; i++){
				if (G.Tab[A.x + i][A.y] == 1) { nboules++; }		
			}	  
			return nboules;
		}
	
	}

	printf("Erreur : Non respect des conditions de lancage.\n");
	return -1; 
	

}

/*----*/
int EstLancable(grille G, boule A, boule B){
// retourne 1 si A est sur l'axe horizontale ou verticale de B et qu'il respecte les conditions pour être lancé, 0 sinon.
	
	if (G.Tab[A.x][A.y] != 1) {
		printf("Erreur : Pas de boule en A !\n");
		return 0;
	}
	else if (G.Tab[B.x][B.y] != 1) {
		printf("Erreur : Pas de boule en B !\n");
		return 0;
	}
	else if (EstACote(A,B)==1) { 
		printf("Erreur : A et B sont cote à cote !\n");
		return 0;
					}
	else if ( !(A.x == B.x || A.y == B.y) ) {
		printf("Erreur : A et B ne sont pas sur le même axe !\n");
		return 0;
	}
	
	return 1;

}

/*----*/
grille Lancee(grille G, boule A, boule B) {
//Sur la grille G, lance la boule A sur la boule B, et renvoie la nouvelle grille avec la direction prise.
	
	if ( (EstLancable(G, A, B)==1) && (Obstacle(G, A, B)==0) ){
	
		// Si A est en bas de B
		if (A.x == B.x && A.y > B.y) {
			G.Tab[A.x][A.y] = 0; //A n'est plus à sa position initiale
			G.Tab[A.x][B.y + 1] = 1; //A est à sa nouvelle position
			A.y = B.y + 1; //On actualise la nouvelle position de A	
			G.Tab[B.x][B.y] = 0; //B est sorti du terrain
			G.dir = NORD; // direction prise
					 
			return G;
		} 
		// Si A est en haut de B
		else if (A.x == B.x && A.y < B.y) {
			G.Tab[A.x][A.y] = 0; 
			G.Tab[A.x][B.y - 1] = 1;
			A.y = B.y - 1; 
			G.Tab[B.x][B.y] = 0;
			G.dir = SUD; 
			return G;
		} 
		// si A est à droite de B
		else if (A.y == B.y && A.x > B.x) {
			G.Tab[A.x][A.y] = 0; 
			G.Tab[B.x + 1][A.y] = 1;
			A.x = B.x + 1; 
			G.Tab[B.x][B.y] = 0;
			G.dir = OUEST; 
			return G;
		}
		//si A est à gauche de B
		else if (A.y == B.y && A.x < B.x) {
			G.Tab[A.x][A.y] = 0; 
			G.Tab[B.x - 1][A.y] = 1;
			A.x = B.x - 1; 
			G.Tab[B.x][B.y] = 0; 
			G.dir = EST;
			return G;
		}
	
	}
	printf("Echec lancée...\n"); 
	return G;

}
/*----*/
boule Chercher(grille G, boule A){
//On cherche toutes les boules sur qui on peut lancer A. Ces boules sont répertoriées dans l'historique de A.
	int i, j;
	boule Z;
	
	A.taille = 0;
	
	//Sur la même ligne que A, sans compter celui-ci.
	for(i=A.x+1; i < G.l; i++){
			if ((G.Tab[i][A.y]==1))) { 
				Z.x = i; 
				Z.y = A.y;
				if ( (EstLancable(G, A, Z)==1) && (Obstacle(G, A, Z)==0) ){
					A.Hist[A.taille] = Z;
					A.taille++;
				}
			}
	}
	//Les autres lignes
	for(j=A.y+1; j < G.h; j++){
		for(i=0; i < G.l; i++){
			if ((G.Tab[i][j]==1))) { 
				Z.x = i; 
				Z.y = j;
				if ( (EstLancable(G, A, Z)==1) && (Obstacle(G, A, Z)==0) ){
					A.Hist[A.taille] = Z;
					A.taille++;
				}
		}
	}
	return A;
}
/*----*/
void AfficherHist(boule A){

	int i; 
	
	for(i=0; i < A.taille; i++){
		printf("( %d , %d ) \n", A.Hist[i].x, A.Hist[i].y );
	}
}
/*----*/




