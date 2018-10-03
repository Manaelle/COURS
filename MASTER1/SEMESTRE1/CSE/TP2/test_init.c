#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NB_TESTS 10

int main(int argc, char *argv[]) {
	
	void *result, *resultFB;
	
	fprintf(stderr, "\n\nTest basique de mémoire. J'ai supprimé l'ancien test car pas assez précis.\n"
			"On va allouer/libérer différentes zones afin de tester les cas possibles.\n T (pour 'tête') correspond au pointeur vers la tête de la liste chainée. \n AC (pour 'Adresse Courante') est le FB pointé lors du parcours de la chaine. \n AP (pour 'Adresse Précédente') est le FB précédant AC. \n newFB est tout simplement l'adresse vers la nouvelle zone libre crée. \n Ces infos vont aider (lors d'une éventuelle présentation) à bien montrer la construction de la mémoire en cours d'exécution.\n \n"
 		"(Désolé, ça va être TRES verbeux...)\n\n");
	
	mem_init(get_memory_adr(), get_memory_size());
	
	result = mem_alloc((size_t)1000);
	debug("Alloced %zu bytes at %p\n", (size_t)1000, result);
	
	result = mem_alloc((size_t)500);
	debug("Alloced %zu bytes at %p\n", (size_t)500, result);
	
	resultFB = mem_alloc((size_t)2000);
	debug("Alloced %zu bytes at %p\n", (size_t)2000, resultFB);
	
	result = mem_alloc((size_t)400);
	debug("Alloced %zu bytes at %p\n", (size_t)400, result);
	
	mem_free(resultFB);
	debug("Freeing at %p\n", resultFB);
	
	result = mem_alloc((size_t)500);
	debug("Alloced %zu bytes at %p\n", (size_t)500, result);
	
	printf("\n---------------------------------\n");
	printf("TEST MEM_GET_SIZE : la dernière zone allouée fait bien %zu octets. \n", mem_get_size(result));
	
	
	printf("\n \nFIN !\n");
	return 0;
}
