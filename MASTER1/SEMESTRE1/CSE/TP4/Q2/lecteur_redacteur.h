#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
	pthread_mutex_t m;
	int lecteur; // nombre de lecteurs en cours
	int redacteur; // nombre de redacteurs en cours
	int lecteurA; // nombre de lecteur en attente
	int redacteurA; // nombre de redacteur en attente
	pthread_cond_t AttenteL; // condition d'attente de lecture
	pthread_cond_t AttenteE; // condition d'attente d'ecriture
}lecteur_redacteur_t;

void initialiser_lecteur_redacteur(lecteur_redacteur_t *lr);

void debut_lecture(lecteur_redacteur_t *lr);

void fin_lecture(lecteur_redacteur_t *lr);

void debut_redaction(lecteur_redacteur_t *lr);

void fin_redaction(lecteur_redacteur_t *lr);

void detruire_lecteur_redacteur(lecteur_redacteur_t *lr);
