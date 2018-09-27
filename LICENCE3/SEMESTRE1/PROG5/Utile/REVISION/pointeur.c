#include <stdio.h>
#include <stdlib.h>

int main(){
	
	char* p = NULL;
	int d = 15;
	
	p = malloc(sizeof(char));
	*p = 'a';

	printf("Valeur : [ %d , %d, %d ] \n", *p, p, p+1);
	printf("Test : [ %d , %d ] \n", d, &d);
	
	free(p);
}