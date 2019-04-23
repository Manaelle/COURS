#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5

int main(void)
{
    int i=0;
    int sum=0;
    int *array= malloc(SIZE * sizeof(int));

    if(array[0] != 0){
        printf("stupid test failed\n");
        return -1;
    }

    for(i=0; i<SIZE; i++){
        array[i]= i;
    }

    for(i=0; i<SIZE; i++){
        sum += array[i];
    }

    printf("sum = %d\n", sum);
    
    free(array);

    return 0;
}
