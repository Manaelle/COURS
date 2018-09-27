#include <stdlib.h>
#include <stdio.h>


/*--------------------------------------*/
//PROTOTYPES
int f(int N);


/*--------------------------------------*/
//MAIN : ./Bracelets N
//Prend en argument le nombre de cases dans le bracelet (sa taille)
int main(int argc, char *argv[]) {

	printf("%d", f(argv[1]));
	 
  return 0 ;
}

/*--------------------------------------*/
//FONCTIONS
int AfficherBracelets(int N){
										
	int nb_bracelets;
	int nb_cases, nb_blocs;
	int ibloc, icase;
	int B[N];
	
	nb_bracelets = 0;
	nb_cases =3;
	nb_blocs =0;

	printf("[ ");
	for (int i=0; i <= N-1; i++){
			
			B[i] = 0;
			
			
		}
	}
	
	
	
return nb_bracelets;
}
/*----*/




