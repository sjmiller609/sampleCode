/*
lab3p2f3.c
Author: Steven Miller
Description:
This function determines if either of two ints are a perfect square, returning a boolean int
*/

/*include libraries*/
#include <stdio.h>
#include "lab3p2.h"

int i;

int lab3p2f3(int x, int y){

	// check every square value up to x or y
	// this will also ignore negative ints which is fine
	//  because negative numbers can't be perfect squares

	for(i = 0; i <= x ; i++){

		if(i*i == x){
			return(1);
		}
		if(i*i > x){
			i = x;
		}
	}

	for(i = 0; i <= y; i++){

		if(i*i == y){
			return(2);
		}
		if(i*i > y){
			i = y;
		}

	}

	return(0);
}
