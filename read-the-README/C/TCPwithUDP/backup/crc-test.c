#include "crc.h"
#include <stdio.h>

void printTCP(char* header,int bytes){
	int i = 0;
	for(;i<bytes;i++){
		if(!(i%4)) printf("\n");
		printchar(header[i]);
		printf(" ");
	}
	printf("\n");
}
int test1(){
	char TCP_header[4*6+4];
	int i = 0;
	//fill with a
	for(;i<4*6+4;i++){
		TCP_header[i] = 'f';
	}
	TCP_header[4*6+3] = (char)0x01;
	printTCP(TCP_header,4*6+4);
	printf("inserting checksum\n");
	insertChecksum(TCP_header,4*6+4);
	printf("checksum inserted\n");
	printTCP(TCP_header,4*6+4);
	//test passed
	printf("checking if valid\n");
	if(isValidCRC(TCP_header,4*6+4)) return 1;
	printf("checked\n");
	//test failed
	return 0;
}
int test2(){
	char TCP_header[4*6+4];
	int i = 0;
	//fill with a
	for(;i<4*6+4;i++){
		TCP_header[i] = 'a';
	}
	TCP_header[4*6+3] = (char)0x01;
	printf("inserting checksum\n");
	insertChecksum(TCP_header,4*6+4);
	printf("checksum inserted\n");
	printf("checking if valid\n");
	TCP_header[25] = 'f';
	//test failed
	if(isValidCRC(TCP_header,4*6+4)) return 0;
	printf("checked\n");
	//test passed
	return 1;
}
int main(int argc,char* argv[]){
	if(test1()){
		printf("test 1 passed\n");
	}else{
		printf("test 1 failed\n");
	}
	if(test2()){
		printf("test 2 passed\n");
	}else{
		printf("test 2 failed\n");
	}
	return 0;
}
