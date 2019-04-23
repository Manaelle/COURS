#include <stdio.h>
#include <string.h>
#include <mpi.h>

// Compilation : mpicc -o hello_world hello_world.c

// Exécution : mpirun -n 2 [-hostfile machine_file] ./hello_world
// Le 2 indique le nombre de processus à faire tourner
// On indique ensuite sur quel machines on veut faire tourner les processus [facultatif !]


//----------------- EXEMPLE PROG MPI -------------------
int main(int argc, char *argv[]){

	char msg[20];
	
	int my_rank, my_size;
	MPI_Status status;

	MPI_Init(&argc, &argv); // Initialise l'env d'exécution
	
	//MPI_COMM_WORLD est un communicateur
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); //Permet d'avoir le rang du processus dans MPI_COMM_WORLD
	MPI_Comm_size(MPI_COMM_WORLD, &my_size); //récupère le nombre de processus apparenant au groupe associé à MPI_COMM_WORLD
	
		
		//---------------- EXO 4 ---------------------
		/*char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    
		printf("Hello world from processor %s, rank %d out of %d processors\n",processor_name, my_rank, my_size);*/
		
		//---------------- EXO 5 ---------------------
		float msg2[10];
		if (my_rank == 0) {
				int i;
				for (i=0;i<10;i++){
					msg2[i] = 0.0 + (float)i;
				}
				MPI_Send(msg2, 10, MPI_FLOAT, 1, 99, MPI_COMM_WORLD);
		}
		if (my_rank == 1){
				MPI_Recv(msg2, 20, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, &status);
				printf("I received %f!\n", msg2[0]);
				
				for (i=0;i<10;i++){
					printf("I received %f!\n", msg2[i]);
				}
		}
		//--------------------------------------------
		MPI_Finalize(); // Termine l'exécution de l'env MPI
	}
	
//----------------------------------------------------
//A SAVOIR :
//Créer une clé SSH : ssh-keygen -t rsa
