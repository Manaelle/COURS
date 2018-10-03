#ifndef __MEM_H
#define __MEM_H
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct fb fb;

void mem_init(char* mem, size_t taille);
void* mem_alloc(size_t S);
void mem_free(void* m);
size_t mem_get_size(void *m);

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free));

/* Choix de la strategie et strategies usuelles */
typedef fb* (mem_fit_function_t)(fb* T, size_t s);
fb* mem_fit(mem_fit_function_t *f);

struct fb* mem_fit_first(struct fb*, size_t);

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb*, size_t);
struct fb* mem_fit_worst(struct fb*, size_t);

#endif
