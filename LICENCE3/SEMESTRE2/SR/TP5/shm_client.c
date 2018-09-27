/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27

void main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    /*
     * We need to get the segment named "5678", created by the server.
     */
    key = 5678;

    /* Locate the segment. */
    if ((shmid = shmget(key, 3*sizeof(int) + SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* Now we attach the segment to our data space. */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
		
		while(1){
			//AJOUTE : Idem que shm_server
			c2 = 1;
		  t = 1;
			while ((c1 == 1) && (t == 1));
		  /* Now read what the server put in the memory.*/
		  for (s = shm; *s != '\0'; s++)
		      putchar(*s);
		  putchar('\n');
		
		  /*
		   * Finally, change the first character of the 
		   * segment to '*', indicating we have read 
		   * the segment.
		   */
		  *shm = '*';
		  c2 = 0;
		 }

    exit(0);
}
