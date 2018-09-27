#include <stdio.h>
#include <unistd.h>
#define TMAX 60
#define LAPS 1

int main()
{
	int i;
	for (i=0; i < TMAX; i++){
		
		printf("bip \n");
		sleep(LAPS);
	
	}
	printf("biiiiiip \n");
	return 0;
}
