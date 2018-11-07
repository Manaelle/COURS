#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "lecteur_redacteur.h"

void initialiser_lecteur_redacteur(lecteur_redacteur_t *lr){
	pthread_mutex_init(&lr->m, NULL);
	lr->redacteur = 0;
	lr->lecteur = 0;	
	lr->lecteurA = 0;
	lr->redacteurA = 0;
	pthread_cond_init(&lr->AttenteL, NULL);
	pthread_cond_init(&lr->AttenteE, NULL);
}

void debut_lecture(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	while(lr->redacteur > 0) {
		pthread_cond_wait(&lr->AttenteL, &lr->m);
	}
	lr->lecteur++;
	pthread_mutex_unlock(&lr->m);
}

void fin_lecture(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	lr->lecteur--;
	if(lr->lecteur == 0) {
		pthread_cond_signal(&lr->AttenteL);
	}
	pthread_mutex_unlock(&lr->m);
}

void debut_redaction(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	lr->redacteurA++;
	while(lr->lecteur > 0 || lr->redacteur > 0) {
		pthread_cond_wait(&lr->AttenteE, &lr->m);
	}
	lr->redacteur++;
	lr->redacteurA--;
	pthread_mutex_unlock(&lr->m);
}

void fin_redaction(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	lr->redacteur--;
	if(lr->redacteurA > 0) {
		pthread_cond_signal(&lr->AttenteE);
	} else {		
		pthread_cond_broadcast(&lr->AttenteL);
	}
	pthread_mutex_unlock(&lr->m);
}

void detruire_lecteur_redacteur(lecteur_redacteur_t *lr){
	pthread_mutex_destroy(&lr->m);
	pthread_cond_destroy(&lr->AttenteL);
	pthread_cond_destroy(&lr->AttenteE);
}
