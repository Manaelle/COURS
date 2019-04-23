#include <stdio.h>
#include <string.h>
#include <mpi.h>

// Compilation : mpicc -o broadcast broadcast.c

// Exécution : mpirun -n 2 ./broadcast
// Le 2 indique le nombre de processus à faire tourner
// On indique ensuite sur quel machines on veut faire tourner les processus [facultatif !]

#define PROC_SENDER 0
#define SIZE_MSG 5

//------------------------------------------------------
//Le processus "sender" de rang rang_sender va envoyer un message à tous les autres processus (broadcast)
void sender(void* msg, int size_msg){
	
	MPI_Bcast(msg, size_msg, MPI_INT, PROC_SENDER, MPI_COMM_WORLD); //fonction de broadcast
}
//------------------------------------------------------
//Le processus "receiver" a reçu un message de PROC_SENDER
void receiver(int size_msg){
	MPI_Status status;
	int i;
	int msg[size_msg];
	
  MPI_Recv(msg, size_msg, MPI_INT, PROC_SENDER, 99, MPI_COMM_WORLD, &status);
  
  for (i=0;i<size_msg;i++){
					printf("I received %d!\n", msg[i]);
				}
}
//----------------- EXEMPLE PROG MPI -------------------
int main(int argc, char *argv[]){

	int a[SIZE_MSG]; // tableau d'integer à envoyer
	int i;
	int my_rank, my_size;
	
	//----------- INIT ---------------
	MPI_Init(&argc, &argv); // Initialise l'env d'exécution
	
	//MPI_COMM_WORLD est un communicateur
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //Permet d'avoir le rang du processus dans MPI_COMM_WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &my_size); //récupère le nombre de processus apparenant au groupe associé à MPI_COMM_WORLD
	//--------------------------------
	
		//Ce que fera le thread émetteur (de rang 0)
		if (my_rank == 0) {
				for (i=0;i<SIZE_MSG;i++){
					a[i] = 42 + i;
				}

		}
		
		sender(a, SIZE_MSG); //pas dans le if(my_rank == 0) !
		//--------------------------------
		//Ce que feront les autres (récepteurs)
		for (i=0;i<SIZE_MSG;i++){
					printf("Processus %d : I received %d!\n", my_rank, a[i]);
				}

		//--------------------------------------------
		MPI_Finalize(); // Termine l'exécution de l'env MPI
	}
	
//----------------------------------------------------
//A SAVOIR :
//Créer une clé SSH : ssh-keygen -t rsa
