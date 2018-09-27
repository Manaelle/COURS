#include "memory_operations.h"
#include <stdlib.h>

//------------------------------------------------------
void *my_memcpy(void *dst, const void *src, size_t len)
{
  int i = 0;
	char *c = dst;
	const char *d = src;
	if ((dst+len < src) || (src+len < dst)) {
		while (i<len) {
			*c=*d;
			c++;
			d++;
			i++;
		}
	}
	return NULL;
}
//------------------------------------------------------
void *my_memmove(void *dst, const void *src, size_t len)
{
  int i = 0;
  void *tmp = malloc(len*sizeof(char));
  char *t;
	char *c = dst;
	const char *d = src;
	
	m
	free(tmp);
	return NULL;
}
//------------------------------------------------------
int is_little_endian()
{
  int resultat = 0;
  return resultat;
}
//------------------------------------------------------
int reverse_endianess(int value)
{
  int resultat = 0;
  return resultat;
}
