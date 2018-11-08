#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "lecteur_redacteur.h"

// Implementation de FIFO

typedef struct cell cell;
struct cell {
	int type; // 1 = lecteur / 2 = redacteur
	pthread_cond_t cond;
	cell *next;
};

cell *front;
cell *back;

// enfiler dans la FIFO
pthread_cond_t * FIFO_add(int type){

	cell* c = malloc(sizeof(cell));
	c->type = type;
	pthread_cond_init(&c->cond, NULL);
	c->next = NULL;

	if(front == NULL) {
		front = c;
	} else {
		tail->next = c;
	}
	tail = c;

	return &c->cond;
}

// defiler de la FIFO
void FIFO_remove(){
	cell *c = front;
	if (front != NULL) {
		front = front->next;
		pthread_cond_destroy(&c->cond);
		free(c);
	}
}

//

void wakeup() {
	if (front->type == 1) { // les suivants sont des lecteurs
		while (front->type == 1) {
			pthread_cond_signal(&front->cond);
			FIFO_remove();
		}
	} else { // les suivants sont des redacteurs
		pthread_cond_signal(&front->cond);
		FIFO_remove();
	}
}

void initialiser_lecteur_redacteur(lecteur_redacteur_t *lr){
	pthread_mutex_init(&lr->m, NULL);
	front = NULL;
	back = NULL;
}

void debut_lecture(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	pthread_cond_t* cond = FIFO_add(1);
	pthread_cond_wait(cond, &lr->m);
	pthread_mutex_unlock(&lr->m);
}

void fin_lecture(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	wakeup();
	pthread_mutex_unlock(&lr->m);
}

void debut_redaction(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	pthread_cond_t* cond = FIFO_add(2);
	pthread_cond_wait(cond, &lr->m);
	pthread_mutex_unlock(&lr->m);
}

void fin_redaction(lecteur_redacteur_t *lr){
	pthread_mutex_lock(&lr->m);
	wakeup();
	pthread_mutex_unlock(&lr->m);
}

void detruire_lecteur_redacteur(lecteur_redacteur_t *lr){
	pthread_mutex_destroy(&lr->m);
}
