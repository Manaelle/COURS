#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
//Compilation : gcc -Wall -fopenmp -o exemple_openMP exemple_openMP.c

int maFonction(int a, int b, int c, int d, int e, int f){
	
	int S, x1, x2, x3; //ces variables sont partagées (par défaut)
	
	omp_set_num_threads(3); //Permet de décider combien de threads je veux
	
	#pragma omp parallel
	{
		#pragma omp single
		{	
			//TACHE 1
			#pragma omp task 
			{
				printf("TACHE 1 : calcul de x1 (%d*%d = %d) fait par le thread %d \n", a,b,a*b, omp_get_thread_num());
				x1 = a*b;
			}
			//TACHE 2
			#pragma omp task 
			{
				printf("TACHE 2 : calcul de x2 (%d*%d = %d) fait par le thread %d \n", c,d,c*d, omp_get_thread_num());
				x2 = c*d;
			}
			//TACHE 3
			#pragma omp task 
			{
				printf("TACHE 3 : calcul de x3 (%d*%d = %d) fait par le thread %d \n", e,f,e*f, omp_get_thread_num());
				x3 = e*f;
			}
			//On attend que toutes les tâches soient terminées avant de calculer S
			#pragma omp taskwait
			S = x1 + x2 + x3;
		}
	}
	return S;
}

int main(int argc, char *argv[]){
		
		int a,b,c,d,e,f;
		
		if (argc == 7){
			a = atoi(argv[1]);
			b = atoi(argv[2]);
			c = atoi(argv[3]);
			d = atoi(argv[4]);
			e = atoi(argv[5]);
			f = atoi(argv[6]);			
			
			int S = maFonction(a,b,c,d,e,f);
			
			printf("RESULTAT (S = x1+x2+x3) : %d \n", S);
		
		}
		
		
	
		return 0;
}

