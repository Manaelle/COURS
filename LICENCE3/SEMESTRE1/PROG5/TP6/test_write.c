#include "bfile.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  BFILE *bfichier;
  int c;
 
  bfichier = bstart(stdout,"w");
  if (bfichier == NULL)
    {
      fprintf(stderr,"Erreur d'ouverture d'acces binaire en ecriture\n");
      exit(3);
    }
  c = getchar();
  while (!feof(stdin))
    {
      switch (c)
        {
        case '0':
          bitwrite(bfichier,0);
          break;
        case '1':
          bitwrite(bfichier,1);
          break;
        }
      c = getchar();
    }
  bstop(bfichier);
  return 0;
}
