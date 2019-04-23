# Rendu TP1 de PAP

## Introduction
Dans la compile : gcc -O0 -fopenmp -o exo2 exo2.c
-O0 indique le niveau d'optimisation : 0 pour aucune optimisation, 1 pour un peu, 2 pour un peu plus etc...

## Première partie

pragma omp parallel num_threads (nthreads) permet d'exécuter une partie du code en parallèle : 
Plusieurs threads sont crées par le master (équivalent d'un fork), puis exécutent tous le même bout de code, ici Hello("Region 1 ")

num_threads (nthreads) va spécifier le nombre de threads voulus. Si cette valeur vaut zéro, openMP se fie au nombre de threads MAX.

## Deuxième partie


Les doubles sont sur 64 bits (8 octets), donc un vecteur est sur N*64 bits et une matrice est sur N*N*64 bits.

On remarque également une variable globale : static long long unsigned int experiment[NBEXPERIMENTS] ;
Les long long unsigned int sont codés sur 64 bits, soit NBEXPERIMENTS*64 bits


### Opération vecteurs

Le temps d'exécution est mesuré en nombre de cycles. On retrouve ces temps à l'affichage :

=============== ADD ==========================================
OpenMP static loop 17238 cycles
OpenMP dynamic loop 151338 cycles

====================DOT =====================================
OpenMP static loop dot 1.024000e+03: 19774 cycles
OpenMP dynamic loop dot 1.024000e+03: 125581 cycles
OpenMP static unrolled loop dot 1.024000e+03: 19098 cycles

(Pour dot, la 1ère valeur est le résultat)

Il faut alors faire : N/(nb_cycles* 1/F) =  0.0098058 sec pour chaque N
... où N = 512 (taille vecteur)
			et F = 900 062 000 (cpu Hz)
			et nb_cycles = 17238
			
NB : pour connaitre les info du CPU, c'est cat /proc/cpuinfo


