#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

int main(void)
{
    int i=0;
    int sum=0;
    int *array[SIZE];

    // Allocation and fill data
    for(i=0; i<SIZE; i++)
    {
        array[i]	= malloc(sizeof(int));
        *(array[i])	= i;
    }

    // Compute and partial free
    for(i=0; i<SIZE; i++)
    {
        sum += *(array[i]);
	if (i % 2 == 0)
	{
		free(array[i]);
	}
    }

    printf("sum = %d\n", sum);
    
    // Free the remaining data
    for(i=0; i<SIZE; i++)
    {
	if ((i % 2 != 0) && (i % 3 == 0))
	{
		free(array[i]);
	}
    }

    return 0;
}
