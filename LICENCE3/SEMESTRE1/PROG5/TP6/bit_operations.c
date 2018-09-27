#include "bit_operations.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------
int get_bit(int A, int x){
//Renvoie le xième bit de A

	return (A >> x) & 1;
}
//---------------------------------------------
int set_bit(int A, int x){
// renvoie A où son xième bit a été mis à 1
	int B = 1;
	
	B = (B << x);
	
	A = A | B;
	
	return A; 
}
//---------------------------------------------
int clr_bit(int A, int x){
// renvoie A où son xième bit a été mis à 0
	int B = 1;
	
	B = (B << x);
	
	B = ~B;
	
	A = A & B;
	
	return A; 
}
