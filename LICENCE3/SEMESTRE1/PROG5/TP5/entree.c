#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char c;
	int nbmot = 0;
	
	printf("Vasy tape un truc. \n");
	scanf("%c", &c);
	
	while( c != '0')
	{
		if( (c == ' ') || (c=='0') ) { nbmot++; } 
		scanf("%c", &c);
	}

  printf("On a %d mots. \n", nbmot);
  return 0;
}
