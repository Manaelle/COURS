#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  long int somme, i;

  somme = 0;
  for (i=1; i<argc; i++)
      {somme = somme + (long int) argv[i];}
  printf("Somme : %ld \n", somme);
  return 0;
}
