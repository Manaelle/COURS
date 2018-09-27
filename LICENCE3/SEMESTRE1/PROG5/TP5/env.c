#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;
int main(int argc, char *argv[])
{
  int i;
  char *chaine;
  
  chaine = malloc(sizeof(char));
  for (i=1; i<argc; i++)
      {
			chaine=realloc(chaine, sizeof(char)*(strlen(chaine)+1+strlen(argv[i])));
			strcat(strcat(chaine," "),argv[i]);
			
      }
   i=setenv("ARGUMENT",chaine,0);
   i=0;
   
  while(environ[i]) {
  	printf("%s\n", environ[i++]);
	}
	free(chaine);

   
  return 0;
}
