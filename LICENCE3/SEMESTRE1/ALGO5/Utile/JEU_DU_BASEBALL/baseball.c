#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NMAX 10
#define A1 1
#define A2 2
#define B1 3
#define B2 4
#define C1 5
#define C2 6
#define D1 7
#define D2 8
#define E 9
#define VIDE 10

//Génère aléatoirement un jeu de baseball, où les pions sont numérotés de 1 à 9 :
// pion 1 et 2 = Bleu
// pion 3 et 4 = Vert
// pion 5 et 6 = Rouge
// pion 7 et 8 = Jaune
// pion 9 = Gris
// 10 est la case vide.
// De même, les bases sont réparties comme suit :
// T[ Bleu1, Bleu2, Vert1, Vert2, Rouge1, Rouge2, Jaune1, Jaune2, Gris1, Gris2 ]

//---------------------------------------
void Echanger(int T[NMAX], int a, int b){
	
	int tmp;
	tmp = T[a];
	T[a]= T[b];
	T[b] = tmp;
	
}
//---------------------------------------
//Initialiser le tableau avec des pions organisés de façon aléatoire
void Init(int T[NMAX]){
	
	int i, tmp, a, b;
	
	srand(time(NULL));
	
	for(i=0; i < NMAX ; i++){
		T[i] = i+1;
	}
	
	//"mélange" des cases :
	for(i=0; i < NMAX ; i++){
		a=rand()% i; // génère uniformément un entier entre 0 et i-1
		Echanger(T,a,i-1);
	}
}
//---------------------------------------
void Afficher(int T[NMAX]){
	
	int i;
	
	printf("[");
	for(i=0; i < NMAX;i++){
		
		if(i != 0){printf(" ; ");}
		
		switch(T[i])
		{
			case 1 : 
				printf("A1");
				break;
			case 2 : 
				printf("A2");
				break;
			case 3 : 
				printf("B1");
				break;
			case 4 : 
				printf("B2");
				break;
			case 5 : 
				printf("C1");
				break;
			case 6 : 
				printf("C2");
				break;
			case 7 : 
				printf("D1");
				break;
			case 8 : 
				printf("D2");
				break;
			case 9 : 
				printf("E");
				break;
			default :
				printf("VIDE");
				break;
		}
		
	}
	printf("]\n");
}
//---------------------------------------
//POSITION_PION : retourne l'indice du pion p
int Position_Pion(int T[NMAX], int p){
		
	int i = 0;
	
	while( (i < NMAX) && (T[i] != p)){			
		i++;
	}
	
	return i;		
}
//-----------
//DETECTON : retourne l'indice du pion qui suit le pion d'indice iQ
int Detection(int T[NMAX], int iQ){
		
	int i = 0;
	
	while( (i < NMAX) && (T[i] != T[iQ]+1)){			
		i++;
	}
	
	return i;		
}
//---------------------------------------
//RAMENER : ramène le pion p2 derrière le pion p1 (tri)
void Ramener(int T[NMAX], int p1, int p2){
		
	//ETAPE 1 : On ramène la case vide devant p2 par décalage à gauche
	while((Position_Pion(T,p2) != Position_Pion(T,VIDE)+1) && (Position_Pion(T,VIDE)!=NMAX-1 || Position_Pion(T,p2)!=0) ){
		
		//le tableau est circulaire !
		if(Position_Pion(T, VIDE) == 0){
			Echanger(T, Position_Pion(T, VIDE), NMAX-1);
		}
		else{
			Echanger(T, Position_Pion(T, VIDE), Position_Pion(T, VIDE) - 1);
		}		
	}
	
	Afficher(T);
	
	//ETAPE 2 : On décale les pions pour ramener P2 derrière P1, par décalage à droite
	while(Position_Pion(T,p2) -1 != Position_Pion(T,p1) && ((Position_Pion(T,p1) != NMAX-1) || (Position_Pion(T,p2) != 0)) ){
		
		if(Position_Pion(T, p2) != NMAX-1){
			Echanger(T, Position_Pion(T, p2) + 1, Position_Pion(T, VIDE));
		}
		else {
			Echanger(T, 0, Position_Pion(T, VIDE));
		}
		
		if ((Position_Pion(T,p2) -1 != Position_Pion(T,p1) && ((Position_Pion(T,p1) != NMAX-1) || (Position_Pion(T,p2) != 0)) )){
			Echanger(T, Position_Pion(T, VIDE), Position_Pion(T, p2));
		}
			
	}
	
	Afficher(T);
}
//---------------------------------------
//PLACEMENT : Effectue la première étape de la résolution du jeu, cad l'enchainement dans l'ordre de tous les pions. (cf feuille)
void Placement(int T[NMAX]){
	
	int i;
	
	//i correspond aux pions A1, A2 etc...
	for(i=2; i < NMAX;i++){
		
		Ramener(T, i-1, i);
	}
}
//---------------------------------------
//ROTATION : 2ème étape du jeu, on tourne les pions jusqu'à qu'ils atteignent la bonne base.
void Rotation(int T[NMAX]){
	
	int i; 
	
	while( T[0] != A1 ){
		
		//décalage (circulaire) à gauche de tous les pions.
		for(i=1; i < NMAX;i++){
			Echanger(T, Position_Pion(T, i) , Position_Pion(T, VIDE));
		}
	}
	Afficher(T);
}
//---------------------------------------
int main(){
	
	int bases[NMAX];
	
	printf("----ETAT INITIAL-----\n");
	Init(bases);
	Afficher(bases);
	printf("-----PLACEMENT------\n");

	Placement(bases);
	
	printf("-----ROTATION------\n");
	
	Rotation(bases);
	
	printf("-----RESOLU !------\n");
	
	return 0;
}
//---------------------------------------