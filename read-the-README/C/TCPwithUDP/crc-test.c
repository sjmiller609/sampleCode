#include "crc.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void printchar(char a){
	int i;
  for (i = 0; i < 8; i++) {
      printf("%d", !!((a << i) & 0x80));
  }
}
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

void fill_random(char* buffer,int bytes){
	int i =0;
	for(;i<bytes;i++){
		buffer[i] = rand();
	}
}
void trollify(char* buffer, int bytes){

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
	int bytes_data = 4;
	char TCP_header[4*6+bytes_data];
	char TCP_header_og[4*6+bytes_data];
	int i = 0;
	int numtests = 10;
	for(;i<numtests;i++){
		fill_random(TCP_header,4*6+bytes_data);
		insertChecksum(TCP_header,4*6+bytes_data);
		bcopy(TCP_header,TCP_header_og,4*6+bytes_data);
		trollfy(TCP_header,4*6+bytes_data);
		if(strcmp(TCP_header,TCP_header_og)){
			/*not equal, troll changed*/
			if(isValidCRC(TCP_header,4*6+bytes_data)){
				printf("failed!!!!!!!\n");
			}else{
				printf("passed\n");
			}
		}else{
			if(!isValidCRC(TCP_header,4*6+bytes_data)){
				printf("failed!!!!!!!\n");
			}else{
				printf("passed\n");
			}
		}
	}
		
/*
	if(
	!!(isSameAfterTroll(TCP_header,4*6+bytes_data)) ==
	!!(isValidCRC(TCP_header,4*6+bytes_data))
	) printf("passed\n");
*/
	
	return 0;
}
