#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27
#define SIZE_C     1

void main()
{
    char c;
    
    //Variables pour shm
    int shmid;
    key_t key;
    char *shm, *s;
    int c1, c2, t;

    /* We'll name our shared memory segment"5678 */
    key = 5678;

    /* Create the segment. */
    //AJOUTE : Espace pour c1, c2 et t.
    if ((shmid = shmget(key, (3*sizeof(int) + SHMSZ), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    

    /* Now we attach the segment to our data space. */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    while(1){
		  //AJOUTE : Contrôle de disponibilité
		  c1 = 1;
		  t = 2;
			while ((c2 == 1) && (t == 2));
		  /* SECTION CRITIQUE : Now put some things into the memory for the other process to read. */
		  s = shm;
		
		  for (c = 'a'; c <= 'z'; c++)
		      *s++ = c;
		  *s = '\0';

		  /*
		   * Finally, we wait until the other process changes the first character of our memory
		   * to '*', indicating that it has read what we put there.
		   */
		   c1 = 0;
		  while (*shm != '*')
		      sleep(1);
		}
		
    exit(0);
}
