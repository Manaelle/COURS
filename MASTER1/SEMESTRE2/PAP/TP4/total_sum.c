#include <stdio.h>
#include <string.h>
#include <mpi.h>

// Compilation : mpicc -o total_sum total_sum.c

// Exécution : mpirun -n 2 ./total_sum
// Le 2 indique le nombre de processus à faire tourner

#define PROC_SENDER 0
#define SIZE_MSG 5

//----------------- EXEMPLE PROG MPI -------------------
int main(int argc, char *argv[]){

	int sum = 0; //variable globale où tous les proc vont y ajouter leur rang
	int my_rank, my_size;
	
	//----------- INIT ---------------
	MPI_Init(&argc, &argv); // Initialise l'env d'exécution
	
	//MPI_COMM_WORLD est un communicateur
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //Permet d'avoir le rang du processus dans MPI_COMM_WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &my_size); //récupère le nombre de processus apparenant au groupe associé à MPI_COMM_WORLD
	//--------------------------------
	MPI_Reduce(&my_rank,&sum, 1, MPI_INT, MPI_SUM, PROC_SENDER, MPI_COMM_WORLD);
	printf("Processus : %d \n", my_rank);
	if (my_rank == 0){
		printf("Somme globale : %d \n", sum);
	}
		
	//------------ END ---------------
		MPI_Finalize(); // Termine l'exécution de l'env MPI
	}
	
//----------------------------------------------------
//A SAVOIR :
//Créer une clé SSH : ssh-keygen -t rsa
