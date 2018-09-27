#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pile.h"
#include "generateur_test.h"
#define MAXCHAINE 10000

//BUT : Créer un programme qui génère dans plusieurs fichiers textes, des tests de parenthésages. Il existera deux types de tests : les bons et les mauvais.

//----------------------------------------
// VERIFICATION BON PARENTHESAGE : prend un fichier.txt contenant un parenthesage lambda, renvoie 1 si le parenthesage est bon, 0 sinon.
int est_bon_parenthesage(const char *nomf )
		{
			pile T ;
			char c;
			FILE* f = NULL;
			
			T = PileVide();
			f = fopen(nomf, "r");
			fscanf(f,"%c", &c);

			while ( !feof(f) )
			{
					
					switch(c) {
						case '(' : T = Empiler(T, c);
											break;

						case ')' : if(EstVide(T)) {
													fclose(f);
													return 0;
											}
											else { 
													T = Depiler(T); 
											}
											break;
					}
				fscanf(f," %c", &c);
			
			}
			
			if(EstVide(T)) { fclose(f);
													return 1; } 
			else {fclose(f);
						return 0;}
					
}
//----------------------------------------
// GENERATEUR BON PARENTHESAGE : Génère un fichier nom.txt contenant un bon parenthesage aléatoire.
void generateur_bon_parenthesage(const char *nomf )
		{
			pile T ;
			int a, l, lmax, nbp, i ;
			FILE* f = NULL;

			T = PileVide();
			f = fopen(nomf, "w");
			srand(time(NULL));

			lmax = rand()%MAXCHAINE; //définit la longueur ALEATOIRE de la chaine de caractère. Ici, on aura entre 0 et MAXCHAINE caractères.
			
			l = 0;
			nbp = 0;

			while ( l <= lmax )
			{
					a = rand()%3; //'a' choisit au hasard de mettre une parenthèse ou un caractère lambda (ici 'a')

					switch(a) {
						case 1 : T = Empiler(T, '(');
											nbp++;			// nbp compte le nombre d'élément dans la pile : servira à savoir le nombre de parenthèse à fermer si la pile n'est pas encore vide.
											fprintf(f, "(");
											break;

						case 2 : if( !EstVide(T) ) {
													T = Depiler(T);
													nbp--;
													fprintf(f, ")"); 
											}
											
											break;

						case 0 : fprintf(f, "a");
											break;
					}
				
				l++; 
			}
			
			// On complète ici les parenthèses fermantes qui manquent. La longueur ne sera pas forcément < MAXCHAINE, mais tant pis !
			if(!EstVide(T)) {
					for (i = 0 ; i < nbp ; i++) {
							T = Depiler(T);
							fprintf(f, ")");
					} 
					fclose(f); 
			} 
			else { fclose(f); }
			printf("Création de fichier à bon parenthesage. Longueur de la chaine :  %d \n", lmax+nbp+1);
					
}		

// GENERATEUR MAUVAIS PARENTHESAGE : Génère un fichier nom.txt contenant un mauvais parenthesage aléatoire.
//----------------------------------------
void generateur_mauvais_parenthesage(const char *nomf )
		{

			int a, l, lmax;
			FILE* f = NULL;

			f = fopen(nomf, "w");
			srand(time(NULL));

			lmax = rand()%MAXCHAINE; 
			
			l = 0;

			while ( l <= lmax )
			{
					a = rand()%3; 
					
					// Je mise ici sur le fait que la probabilité de tomber sur un bon parenthesage est faible. Je vérifie malgré tout cela au parenthesage.c.
					switch(a) {
						case 1 : 	fprintf(f, "(");
											break;

						case 2 :	fprintf(f, ")"); 
											break;

						case 0 : fprintf(f, "a");
											break;
					}
				
				l++; 
			}
			
			printf("Tentative de création de fichier à mauvais parenthesage. Longueur de la chaine :  %d \n", lmax+1);
			fclose(f);		
}	

