#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NB_TESTS 10

/*static void *alloc_max(size_t estimate) {
	void *result;
	static size_t last = 0;

	while ((result = mem_alloc(estimate)) == NULL) {
		estimate--;
	}
	debug("Alloced %zu bytes at %p\n", estimate, result);
	if (last) {
		// Idempotence test
		assert(estimate == last);
	} else
		last = estimate;
	return result;
}*/

int main(int argc, char *argv[]) {
	
	void *result, *resultFB;
	
	fprintf(stderr, "Test réalisant de multiples fois une initialisation suivie d'une alloc max.\n"
			"Définir DEBUG à la compilation pour avoir une sortie un peu plus verbeuse."
 		"\n");
	
	/*for (int i=0; i<NB_TESTS; i++) {
		mem_init(get_memory_adr(), get_memory_size());
		alloc_max(get_memory_size());
	}*/

	// TEST OK
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
	
	//mem_free(result);
	//debug("Freeing at %p\n", result);
	
	printf("FIN");
	return 0;
}
