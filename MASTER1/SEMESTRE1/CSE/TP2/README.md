# CSE : Gestion Mémoire

## Compilation :

> make

Le makefile compile le nécessaire et exécute test_init, ce qui permet d'avoir un aperçu direct du test. Pour tester plus en profondeur, exécuter la commande suivante :

> ./memshell

## Fonctionnement globale :

Le programme permet de simuler la gestion mémoire, par l'intermédiaire de deux structures principales :

- la structure FB : Entête d'une zone libre. Elle contient une taille (taille de la zone libre + taille de la structure FB) et un pointeur vers la prochaine zone libre.La dernière zone libre pointe vers NULL.

- La structure UB : Entête d'une zone allouée. Elle contient la taille UTILISABLE de la zone allouée (la taille de la structure UB y est exclue).

Le pointeur T pointe vers la première zone libre (= tête de la liste chainée de zones libres), et est stocké dans la première case de la Mémoire. 

## Fonctions disponibles et opérationnelles :

> void mem_init(char* mem, size_t taille);

Initialise la mémoire en plaçant T en première case, puis une structure FB. 

> void* mem_alloc(size_t S);

Prend en paramètre une taille S à allouer. Cherche une zone libre assez grande pour permettre l'allocation grâce aux méthodes mem_fit_first/best/worst, puis alloue la zone en créant une entête UB.

> void mem_free(void* m);

Libère la zone pointée par m, en créant une nouvelle entête FB à la place de UB. Si des zones libres qui se suivent apparaissent, une fusion est effectuée.

LIMITE : Si l'utilisateur entre un pointeur m qui ne pointe PAS vers une zone allouée, des erreurs peuvent survenir. 
 
> size_t mem_get_size(void *m);

Renvoie la taille de la zone allouée m en prenant en compte les fusions de zones libres possibles.

> void mem_show(void (*print)(void *, size_t, int free));

Permet d'afficher des informations sur l'état de la Mémoire. "print" est une fonction dans memshell.c qui permet de montrer les zones libres, allouées, ou les deux en même temps. 

> -----------------------------------------------------

> struct fb* mem_fit_first(struct fb*, size_t);

Cherche la PREMIERE zone libre assez grande pour permettre l'allocation.

> struct fb* mem_fit_best(struct fb*, size_t);

Cherche la zone libre la plus petite possible pour minimiser les résidus.

> struct fb* mem_fit_worst(struct fb*, size_t);

Cherche la zone libre la plus grande possible pour espérer qu'une éventuelle fusion permettra d'éliminer les résidus.


