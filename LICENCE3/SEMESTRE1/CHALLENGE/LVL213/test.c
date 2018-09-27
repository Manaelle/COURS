#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define NMAX 100


/*--------------------------------------*/
//PROTOTYPES


/*--------------------------------------*/
//MAIN
int main() {

	char buf[20];
	int a, j;
	
	
	printf("Ecrire du texte :\n");
	a = read(0, buf, 20);
	
	printf("Nb de caractères lues : %d \n", a);
	printf("Chaine : %s \n", buf);
	
	for(j=0; j <= a; j++){			
		if(buf[j] == '\n') { printf("saut de ligne !"); }
	}
  
  return 0 ;

}

/*--------------------------------------*/
//FONCTIONS


