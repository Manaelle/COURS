#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAXPRIORITE 1000
#define MAXVAL 100

int main() {

	int p, i, v;
	FILE* f = NULL;
	
	
	f = fopen("jeu_test.txt", "w");
	
	srand(time(NULL));
	
	for(i=0; i<10; i++){
		
		p = rand()%MAXPRIORITE;
		v = rand()%MAXVAL;
		fprintf(f, "i %d %d \n", p, v);
	
	}
	
	for(i=0; i<20; i++){
		
		fprintf(f, "e\n");
	
	}
	
	fprintf(f, "q");
	fclose(f);


return 0;
}
