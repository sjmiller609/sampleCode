/*
lab3p2f1.c
Author: Steven Miller
Description:
This is a power function for integers
*/

/*include libraries*/
// this standard library is not used for the calculation.
#include <stdio.h>
#include "lab3p2.h"

int i;
int result;

int lab3p2f1(int x, int y){

	result = 1;
		
	// multiply one by x y times.
	for(i = 0;i < y;i++){
		result = result*x;
	}
	// if y is negative, then repeatedly divide instead.
	for(i = 0; i > y; i--){
		result = result/y;
	}

	if(y < 0){
		printf("Warning: negative exponent. Result calculated with repeated integer division.\n");
	}

	return(result);
}
