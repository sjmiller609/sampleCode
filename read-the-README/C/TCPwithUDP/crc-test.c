#include "crc.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

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
void trollify(char* buffer, int bytes, int trollport,char* trollhost){
	int sock;
	struct{
		struct sockaddr_in header;
		char body[bytes];
	} message;
	bcopy(buffer,message.body,bytes);

	sock = socket(AF_INET, SOCK_DGRAM,0);
	if(sock < 0) {
		perror("opening datagram socket");
		exit(2);
	}

	message.header.sin_family = AF_INET;
	message.header.sin_port = htons(4567);
	message.header.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sock,(struct sockaddr *)&(message.header),sizeof(message.header))){
		perror("getting socket message.header");
		exit(2);
	}
	message.header.sin_family = htons(AF_INET);
	struct sockaddr_in troll;
	troll.sin_family = AF_INET;
	//troll.sin_addr = message.header.sin_addr;
	troll.sin_port = htons(trollport);

	struct hostent *hp,*gethostbyname();
	hp = gethostbyname(trollhost);
	if(hp == 0) {
		fprintf(stderr, "%s:unknown host\n", trollhost);
		exit(3);
    	}
	bcopy((char *)hp->h_addr, (char *)&troll.sin_addr, hp->h_length);

	/*send to troll*/
	if(sendto(sock, (char*)&message, sizeof(message), 0, (struct sockaddr *)&troll, sizeof(troll)) <0) {
		perror("sending datagram message");
		exit(4);
	}

	bzero(&message, sizeof(message));
	if(recvfrom(sock, &message, sizeof(message), 0,NULL,NULL) < 0) {
		perror("sending datagram message");
		exit(5);
	}
	bcopy(message.body,buffer,bytes);
	/* close connection */
	close(sock);
}
int main(int argc,char* argv[]){
	if(argc<3){
		printf("%s <troll port> <troll host>\n",argv[0]);
		return -1;
	}
/*
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
*/
	int bytes_data = 4;
	char TCP_header[4*6+bytes_data];
	char TCP_header_og[4*6+bytes_data];
	int i = 0;
	int numtests = 10;
	for(;i<numtests;i++){
		fill_random(TCP_header,4*6+bytes_data);
		insertChecksum(TCP_header,4*6+bytes_data);
		bcopy(TCP_header,TCP_header_og,4*6+bytes_data);

		printf("\nsending to troll:");
		printTCP(TCP_header,4*6+bytes_data);
		trollify(TCP_header,4*6+bytes_data,atoi(argv[1]),argv[2]);
		printf("\ngot back");
		printTCP(TCP_header,4*6+bytes_data);
		printf("isValidCRC says: ");
		if(isValidCRC(TCP_header,4*6+bytes_data)){
			printf("true\n");
		}else{
			printf("false\n");
		}
	}
	return 0;
}
