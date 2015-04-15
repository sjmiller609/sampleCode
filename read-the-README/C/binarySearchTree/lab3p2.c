/*
Author: Steven Miller
Partner: None 
Assignment: lab 3
Due: 10 - 8 - 14
Description:
lab 3 part 2 main
user inputs function type and two ints, then calculations are run.
*/

/*include libraries*/
#include <stdio.h>
#include "lab3p2.h"
#define TRUE 1
#define FALSE 0

int result;
int func_type;
int integer1;
int integer2;
//declare function pointer
int (*fp[3])(int,int);


int main(int argc, char* argv[]){

	// make fp[i] point to lab3p2f(i+1)
	fp[0] = lab3p2f1;
	fp[1] = lab3p2f2;
	fp[2] = lab3p2f3;

	// Check to make sure there are the correct number of arguments
	if(argc != 4){
		printf("Incorrect number of arguments! There should be 4 arguments\n");
		printf("lab3p2 [function type] [integer 1] [integer 2]\n");
	return(0);
	}
	
	// Read from argv[], converting to int
	func_type = atoi(argv[1]);
	integer1 = atoi(argv[2]);
	integer2 = atoi(argv[3]);
	
	// print info message for clarity
	printf("You chose function %i with integers %i and %i\n", func_type,integer1,integer2);

	// Check to make sure function type is 1 through 3
	if(func_type < 1 || func_type > 3){
		printf("not a valid function type. the second argument should be 1, 2, or 3.\n");
		return(0);
	}
	
	// Execute function
	result = (*fp[func_type-1])(integer1,integer2);	
	
	// print result if function 1, otherwise use result as boolean and print "YES" or "NO"
	printf("Result of function %i is ",func_type);
	if(func_type == 1){
		printf("%i\n",result);
	}else{
		if(result){
			printf("YES\n");
		}else{
			printf("NO\n");
		}
	}
	return(0);
}
