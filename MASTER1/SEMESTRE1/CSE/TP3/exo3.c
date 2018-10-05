#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>



int *T; // Tableau de valeurs
int taille;
struct section_recherche *section; // Tableau de sections de recherche

// Structure de recherche
struct section_recherche { 
	int to_find;
	int start;
	int end;
};

// Lecture au clavier des valeurs
void lire(){
	int i;

	printf("Entrer taille: ");
	scanf(" %d", &taille);

	printf("\nEntrer valeurs: ");

	T = malloc(taille * sizeof(int));

	for(i=0; i<taille; i++){
		scanf(" %d", &T[i]);
	}
}

//Recherche par sections
void search(void *str){

	struct section_recherche *section = (struct section_recherche *) str;

	printf("To Find : %d / Start : %d / End : %d\n", section->to_find, section->start, section->end);
	
	int i;

	//Recherche
	for (i=section->start; i<=section->end; i++) {
		if (section->to_find == T[i]) {
			printf("Find %i\n", T[i]);
		}
	}
}


int main(int argc, char **argv){

	int i;
	int nb_thread;
	pthread_t *tids;
	int nb_to_find;

	//Gestion des paramètres
	if(argc >= 3){
		nb_thread = atoi(argv[1]);
		tids = malloc(nb_thread * sizeof(pthread_t));
		section = malloc(nb_thread * sizeof(struct section_recherche));
		nb_to_find = atoi(argv[2]);
	} else {
		fprintf(stderr, "Arg 1 : Number of threads / Arg 2 : value to find \n");
		exit(1);
	}

	lire();

	// affichage du tableau lu
	printf("\nValeurs lues : ");
	for(i=0; i<taille; i++){
		printf("%d ", T[i]);
	}
	printf("\n");


	int taille_section = taille / nb_thread;


	for(i=0; i<nb_thread; i++){

		section[i].to_find = nb_to_find;
		section[i].start = i * taille_section;
		section[i].end = (i+1) * taille_section;

		//Création du thread pou chaque i
		pthread_create(&tids[i], NULL, (void * (*)(void *))search, &section[i]);
	}

	//Join des threads
	for(i=0; i<nb_thread; i++){
		pthread_join(tids[i], NULL);
	}

	free(tids);

	exit(0);

}
