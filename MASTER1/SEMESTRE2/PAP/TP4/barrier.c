#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>

#include <mpi.h>

// Compilation : mpicc -o barrier barrier.c

// Exécution : mpirun -n 2 ./barrier
// Le 2 indique le nombre de processus à faire tourner

#define PROC_SENDER 0

//----------------------------------------------------
//cette fonction sera exécutée par chaque processus
void duTafLambda(int maxtime, int rank){
	
	int time_task;
	time_task = rand()%maxtime;
	sleep(time_task);
				
	printf("PROCESSUS %d  (Temps pris : %d) num associé : %d \n", rank, time_task, rand()%1000);
}

//----------------- EXEMPLE PROG MPI -------------------
int main(int argc, char *argv[]){

	int my_rank, my_size;
	
	
	//----------- INIT ---------------
	MPI_Init(&argc, &argv); // Initialise l'env d'exécution
	
	//MPI_COMM_WORLD est un communicateur
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //Permet d'avoir le rang du processus dans MPI_COMM_WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &my_size); //récupère le nombre de processus apparenant au groupe associé à MPI_COMM_WORLD
	
	srand(time(NULL)+my_rank);
	//--------------------------------
	duTafLambda(5, my_rank);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (my_rank == 0){
		printf("Processus 0 : Je crois qu'ils ont tous fini ! \n");
	}
	
	//------------ END ---------------
		MPI_Finalize(); // Termine l'exécution de l'env MPI
	}
	
//----------------------------------------------------
