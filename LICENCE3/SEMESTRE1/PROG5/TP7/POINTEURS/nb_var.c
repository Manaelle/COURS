#include <stdio.h>
#include <stdarg.h>

void essai(int nombre, ...)
{
  int i;
  va_list ap;

  va_start(ap,nombre);
  for (i=0;i<nombre;i++)
      printf("%d\n", va_arg(ap,int));
  va_end(ap);
}

void minimum(int nombre, ...)
{
  int i;
  va_list ap;
  double x,y;

  va_start(ap,nombre);
  x = va_arg(ap,double);

  for (i=1;i<nombre;i++){
	y = va_arg(ap,double);
	if (y < x) {x = y;}
  }

  printf("Le minimum est : %f\n",y);    
  va_end(ap);
}

int main()
{
  essai(4,5,6,7,9);
  minimum(5,0.6,0.2,0.3,1.0,0.1);
  return 0;
}