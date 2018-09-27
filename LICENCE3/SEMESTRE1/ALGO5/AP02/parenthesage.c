#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pile.h"
#include "generateur_test.h"

//Mettre en argument le nom du fichier + 1 si création de tests, 2 si vérification de fichier.

int main(int argc, char *argv[])
{

		char bon[42];
		char mauvais[42];
		char neutre[42];
		int choix;
		
		printf("-----------------------------------\n");
		printf("Tapez 1 pour générer des fichiers de test (un fichier BON et un fichier MAUVAIS sera généré), tapez 2 pour tester un fichier test :\n");
		scanf("%d", &choix);
		
	if (argc == 1){
		
		if ( choix == 1 ) {
		
					printf("Tapez un nom de fichier (sans le .txt) : \n");
					scanf("%s", neutre);
					
					strcpy(bon,neutre);
					strcpy(mauvais,neutre);
					strcat(bon , "_bon.txt");
					strcat(mauvais, "_mauvais.txt");		
		
					generateur_bon_parenthesage( bon );
		
					generateur_mauvais_parenthesage( mauvais );
					
					//Si par malchance on crée un bon test, on refait... Jusqu'à tomber sur un mauvais test.
					while ( est_bon_parenthesage(mauvais) ) {
						printf("Retentative...\n");
						generateur_mauvais_parenthesage( mauvais );
					}
		}
		
		else if (choix == 2 ){
						
						printf("Tapez le nom du fichier à tester (avec le .txt) : \n");
						scanf("%s", neutre);
						printf("-----------------------------------\n");
						printf("Test de fichier : \n");
						
						if ( est_bon_parenthesage(neutre) )
							{
								printf("Bon parenthesage\n");
							}
						else 
						{
							printf("Mauvais parenthesage\n");
						}
				
	
				}
		}
		
		printf("-----------------------------------\n");

return 0;
}
