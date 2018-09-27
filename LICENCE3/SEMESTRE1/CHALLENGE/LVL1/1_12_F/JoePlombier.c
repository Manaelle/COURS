#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define NMAX 50

#define VIDE 0
#define HORSTERRAIN 1
#define N 2
#define S 3
#define NE 4
#define NW 5
#define SE 6
#define SW 7
#define PRIS 8

/*--------------------------------------*/
//TYPES
typedef int direction;

//un tuyau est défini par son type (à quel bout il est relié) + sa direction
typedef struct {
	char t;
	direction dir;
} tuyau ;

typedef struct {
	char t;
	int i, j;
} boutCoord ;

typedef struct {
	int l, h;
	int nb_tuyaux;
	tuyau Tab[NMAX][NMAX];
	boutCoord TabBoutEntree[NMAX];
	int nE;
	boutCoord TabBoutSortie[NMAX];
	int nS;
} grille;

typedef struct {
	int n;
	int L[NMAX];
} liste;
/*--------------------------------------*/
//PROTOTYPES
boutCoord Coordonnees(char c, grille T);
int CaseOK(grille T, boutCoord P);
grille InitGrille();
void AfficherGrille(grille T);
liste cmd();

/*--------------------------------------*/
//FONCTIONS
boutCoord Coordonnees(char c, grille T){
	
	boutCoord B;
	int i;
	B.t = '\0';
	B.i = 0;
	B.j = 0;
	
	for(i=0; i< T.nS ; i++){
		
		if (T.TabBoutEntree[i].t == c){
			B.t = c;
			B.i = T.TabBoutEntree[i].i;
			B.j = T.TabBoutEntree[i].j;
			return B;
		}
		else if (T.TabBoutSortie[i].t == c){
			B.t = c;
			B.i = T.TabBoutSortie[i].i;
			B.j = T.TabBoutSortie[i].j;
			return B;
		}
	}
	
}
/*----*/
int CaseOK(grille T, boutCoord P){
	
	int ok = (T.Tab[P.i][P.j].dir == VIDE) ||
			(T.Tab[P.i][P.j].dir != VIDE)&& (
											(T.Tab[P.i][P.j].dir != PRIS)|| 
											((T.Tab[P.i][P.j].t >= 'a')&&(T.Tab[P.i][P.j].t <= 'z'))
											);
	return ok;
}
/*----*/
liste cmd(){
	
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
grille InitGrille(){
	
	grille T;
	int i, j;
	char c;
	liste I;

	I = cmd();
	
	//c = getc(stdin);
	T.h = I.L[0];
	T.l = I.L[1];
	
	I = cmd();
	T.nb_tuyaux = I.L[0];
	
	T.nE = 0;
	T.nS = 0;
	
	//AJOUT DES BOUTS
	for(j=0; j<T.h; j++){
		for(i=0; i<T.l; i++){
			 
			if (i%2==0) { 
				if (j==T.h-1) { 
								c = getc(stdin);
								T.Tab[i][j].t = c; 
								T.Tab[i][j].dir = VIDE;
								T.Tab[i][j+1].t = '#';
								T.Tab[i][j+1].dir = HORSTERRAIN;
								
				}
				else { 
								c = getc(stdin);
								T.Tab[i][j].t = c; 
								T.Tab[i][j].dir = VIDE;
				}
				
				//Ajout des Entres/Sorties de bout
				if ( (c >= 'A') && (c <= 'Z')){ 
						T.TabBoutEntree[T.nE].t = c;
						T.TabBoutEntree[T.nE].i = i;
						T.TabBoutEntree[T.nE].j = j;
						T.nE++;
				}
				else if( (c >= 'a') && (c <= 'z')){ 
						T.TabBoutSortie[T.nS].t = c;
						T.TabBoutSortie[T.nS].i = i;
						T.TabBoutSortie[T.nS].j = j;
						T.nS++;
				}
			}
			else{
				if (j==0) { T.Tab[i][j].t = '#'; 
							T.Tab[i][j].dir = HORSTERRAIN;
							c = getc(stdin);
							T.Tab[i][j+1].t = c;
							T.Tab[i][j+1].dir = VIDE;
				}
				else { 
							c = getc(stdin);
							T.Tab[i][j+1].t = c;
							T.Tab[i][j+1].dir = VIDE;
				}
				
				//Ajout des Entres/Sorties de bout
				if ( (c >= 'A') && (c <= 'Z')){ 
						T.TabBoutEntree[T.nE].t = c;
						T.TabBoutEntree[T.nE].i = i;
						T.TabBoutEntree[T.nE].j = j+1;
						T.nE++;
				}
				else if( (c >= 'a') && (c <='z')){ 
						T.TabBoutSortie[T.nS].t = c;
						T.TabBoutSortie[T.nS].i = i;
						T.TabBoutSortie[T.nS].j = j+1;
						T.nS++;
				}
			}
		}
		c = getc(stdin); //lecture de '\n'
	}
	
	boutCoord P;
	int OK;
	
	//AJOUT DES TUYAUX
	for(i=0; i<T.nb_tuyaux; i++){
		
		c = getc(stdin);
		P = Coordonnees(c, T);
		T.Tab[P.i][P.j].dir = PRIS;
		
		do{
			c = getc(stdin); //pour le ':'
			switch(c){
				case 'S' : 
						c = getc(stdin);
						switch(c){
							case 'E' : 
										if (P.i%2==0){P.j = P.j +1;}
										P.i = P.i +1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);		
										if ((P.j>T.h) || (P.i >= T.l) || !OK ){
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = SE;
										break;
							case 'W' : 
										if (P.i%2==0){P.j = P.j +1;}
										P.i = P.i -1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);
										if ((P.j>T.h) || (P.i < 0) || !OK ){
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = SW;
										break;	
							default:
										P.j = P.j +1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);
										if ( P.j>T.h || !OK ){
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = S;
										break;
						}
						break;
					
				case 'N' : 
						c = getc(stdin);
						switch(c){
							case 'E' : 
										if (P.i%2==1){P.j = P.j -1;}
										P.i = P.i +1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);
										if ( (P.j<0) || (P.i >= T.l) || !OK ){ 
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = NE;
										break;
							case 'W' : 
										if (P.i%2==1){P.j = P.j -1;}
										P.i = P.i -1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);
										if ( (P.j<0) || (P.i <0) || !OK ){
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = NW;
										break;	
							default:
										P.j = P.j -1;
										//ERREUR : croisement ou sortie de terrain
										OK = CaseOK(T,P);
										if ((P.j<0) || !OK ){
											printf("Montage incorrect");
											return T;
										}
										T.Tab[P.i][P.j].dir = N;
										break;
						}
						break;				
				
					
				default:
					break;
			}
			
			
		}while((c >= '.') && (c <= 'z') );
		
		//ERREUR : Mauvaise connexion
		if (P.t + 32 != T.Tab[P.i][P.j].t) { 
			printf("Montage incorrect");
			return T;
		}
	}
	
	//VERIFICATION DES TROUS
	for(j=0; j<T.h; j++){
		for(i=0; i<T.l; i++){
			//ERREUR : Case vide !
			if (T.Tab[i][j].dir == VIDE) { 
				printf("Montage incorrect");
				return T;
			}
		}
	}
	
printf("Montage correct");		
return T;
}
/*----*/
void AfficherGrille(grille T){
	
	int i, j;
	
	printf("\n");
	for(j=0; j<T.h+1; j++){
		for(i=0; i<T.l; i++){
			
			printf("%c ",T.Tab[i][j].t);	
		
		}
		printf("\n");
	}
	
	printf("----------------\n");
	
	for(j=0; j<T.h+1; j++){
		for(i=0; i<T.l; i++){
			
			printf("%d ",T.Tab[i][j].dir);	
		
		}
		printf("\n");
	}
	return;
}

/*--------------------------------------*/
//MAIN
int main(){
	
	grille G;
	
	G = InitGrille();
	//AfficherGrille(G);
	 
  return 0;
}


