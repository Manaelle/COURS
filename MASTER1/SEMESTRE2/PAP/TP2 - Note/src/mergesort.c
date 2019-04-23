#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<omp.h>

#include <x86intrin.h>

#define NBEXPERIMENTS   7

static long long unsigned int experiments [NBEXPERIMENTS] ;
static   unsigned int N ;
typedef  int  *array_int ;
static array_int X ;

//--------------------------------------
void init_array (array_int T)
{
  register int i ;

  for (i = 0 ; i < N ; i++)
    {
      T [i] = N - i ;
    }
}

//--------------------------------------
void print_array (array_int T)
{
  register int i ;

  for (i = 0 ; i < N ; i++)
    {
      printf ("%d ", T[i]) ;
    }
  printf ("\n") ;
}
//--------------------------------------
int is_sorted (array_int T)
{
  register int i ;
  
  for (i = 1 ; i < N ; i++)
    {
        /* test designed specifically for our usecase */
        if (T[i-1] +1  != T [i] )
            return 0 ;
    }
  return 1 ;
}
//--------------------------------------

long long unsigned int average (long long unsigned int *exps)
{
  unsigned int i ;
  long long unsigned int s = 0 ;

  for (i = 2; i < (NBEXPERIMENTS-2); i++)
    {
      s = s + exps [i] ;
    }

  return s / (NBEXPERIMENTS-2) ;
}
//--------------------------------------
/* inspiré de https://en.wikipedia.org/wiki/Merge_sort */
//Trie un tableau T dont l est le plus petit indice, r le plus grand, et m le milieu
void merge(int *T, int r, int l, int m)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int right[n2];
    int left[n1];

    for (i = 0; i < n1; i++)
        left[i] = T[l + i];
    for (j = 0; j < n2; j++)
        right[j] = T[m + 1+ j];

    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            T[k] = left[i];
            i++;
        }
        else
        {
            T[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        T[k] = left[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        T[k] = right[j];
        j++;
        k++;
    }
}
//--------------------------------------
void merge_sort (int *T, const int size) 
{
    int r=size-1;
    int l=0;
     if (l < r)
     {
         int m = l+(r-l)/2;
         merge_sort(T, size/2);
         merge_sort(T+size/2, size-(size/2));
         merge(T,r,l,m);
     }
}

//--------------------------------------
void parallel_merge_sort (int *T, const int size)
{
    int r=size-1;
    int l=0;
     if (l < r){
			int m = l+(r-l)/2;

			#pragma omp task shared(T)
			{
				parallel_merge_sort(T, size/2);
				
			}
			#pragma omp task shared(T)
			{
				parallel_merge_sort(T+size/2, size-(size/2));
			}
			#pragma omp taskwait
			merge(T,r,l,m);
		
	}
}

//--------------------------------------------------
int main (int argc, char **argv)
{
  unsigned long long int start, end, residu ;
  unsigned long long int av ;
  unsigned int exp ;

    if (argc != 2)
    {
      fprintf (stderr, "mergesort N \n") ;
      exit (-1) ;
    }

  N = 1 << (atoi(argv[1])) ;
  X = (int *) malloc (N * sizeof(int)) ;

  printf("--> Sorting an array of size %u\n",N);
  
  start = _rdtsc () ;
  end   = _rdtsc () ;
  residu = end - start ; 

  
  printf("sequential sorting ...\n");


    for (exp = 0 ; exp < NBEXPERIMENTS; exp++)
    {
      init_array (X) ;
      start = _rdtsc () ;

               merge_sort (X, N) ;
     
      end = _rdtsc () ;
      experiments [exp] = end - start ;
      
      if (! is_sorted (X))
	{
            fprintf(stderr, "ERROR: the array is not properly sorted\n") ;
            exit (-1) ;
	}      
    }
  
  av = average (experiments) ;  
  printf ("\n merge sort serial\t\t %Ld cycles\n\n", av-residu) ;
  //print_array (X) ;

  printf("parallel sorting ...\n");
  
  for (exp = 0 ; exp < NBEXPERIMENTS; exp++)
    {
      init_array (X) ;
      
      start = _rdtsc () ;
		
		//----------------ZONE PARALLELE-------------------
		omp_set_nested(1); //Diminue le nombre de cycles
		omp_set_num_threads(2); //testé avec 4, 8, 16 threads. Pas + efficace.
		omp_set_dynamic(0);
		#pragma omp parallel
		 {
			#pragma omp single nowait
			 {
				parallel_merge_sort (X, N) ;
			 }
		 }
		 //------------------------------------------------
     
      end = _rdtsc () ;
      experiments [exp] = end - start ;

      if (! is_sorted (X))
	{
            fprintf(stderr, "ERROR: the array is not properly sorted\n") ;
            exit (-1) ;
	}      
    }
  
  av = average (experiments) ;
  printf ("\n merge sort parallel with tasks\t %Ld cycles\n\n", av-residu) ;
  //print_array (X) ;
  return 0;
  
}
