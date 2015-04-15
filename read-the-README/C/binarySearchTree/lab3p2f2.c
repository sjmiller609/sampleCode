/*
lab3p2f2.c
Author: Steven Miller
Description:
This function determines divisiblity, returning a boolean int
*/

/*include libraries*/
/*standard library not used in calculation*/
#include <stdio.h>
#include "lab3p2.h"

int lab3p2f2(int x, int y){
	
	// check if y = 0
	if(y == 0){
		printf("Warning: testing divisiblity by zero\n");
		return(0);
	}

	// x % y = 0 only if x is diviside by y.
	return(!(x % y));
}
