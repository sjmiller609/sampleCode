//header file to implement the CRC functions.
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
void printchar(char a){
	int i;
  for (i = 0; i < 8; i++) {
      printf("%d", !!((a << i) & 0x80));
  }
}
void shift_right(char *ar, int size,int start)
{
char* c = &(ar[start/8]);
uint32_t* p32 = (uint32_t*)c;
*p32 = htonl((ntohl(*p32) >> 1));

//	 *((uint32_t*) &(ar[start/8]))= htonl(*((uint32_t*) &(ar[start/8]))) >> 1;
}   

/*
void xorAtIndex(int index,char* buffer,char* poly){
	char temp[3];
	bcopy(poly,temp,3);
	shift_right(temp,3,index%8);
	buffer[index/8] = buffer[index/8] ^ temp[0];
	buffer[index/8+1] = buffer[index/8+1] ^ temp[1];
	buffer[index/8+2] = buffer[index/8+2] ^ temp[2];
}
*/

//polynomial: 1100 0000 0000 0010 1(000 0000)
//	0x			C	0    0     2   8	 0
void calcChecksum(char* TCP_header, int packetsize, char* checksum){
	//initialize polynomial in 3 bytes, 17 bits of which matter
	char working_buffer[packetsize + 2];
	bcopy((void *) TCP_header,(void*)working_buffer,packetsize);
	bzero((void*)(working_buffer+packetsize),2);

	char poly[packetsize+2];
	bzero((void*)poly,packetsize+2);
	poly[0] = (char)0xC0;
	poly[1] = (char)0x02;
	poly[2] = (char)0x80;
	int i = 0;
	int j = 0;
	//printf("working_buffer = \n");
	for(;i<packetsize*8;i++){
		//if(!(i%8)) printf(" ");
		//if(!(i%32)) printf("\n");
		if(
			working_buffer[i/8]
				&
			(1<<(7-i%8))
		){//if bit i is 1
		//	printf("1");
			for(j=i/8;j<(i/8+4);j++){
				working_buffer[j] = working_buffer[j]^poly[j];
			}
		}else{
		//	printf("0");
		}
		/*
		printf("shifting...\n");
		for(j=0;j<packetsize+2;j++){
			printchar(poly[j]);
		}
		printf("\n");
		*/
		shift_right(poly,packetsize+2,i);
	}
	//printf(" ");
//TODO: remove
	//printchar(working_buffer[packetsize]);
	//printchar(working_buffer[packetsize+1]);
//--------------------
	//printf("\n");
	bcopy((void*)(working_buffer+packetsize),(void*)checksum,2);
	//checksum[0] = ~checksum[0];
	//checksum[1] = ~checksum[1];
//--------------------
	/*
	printf("checksum = ");
	printchar(checksum[0]);
	printf(" ");
	printchar(checksum[1]);
	printf(" ");
    printf("\n");
*/
//--------------------
}

//CRC-16
void insertChecksum(char* TCP_header,int packetsize/*bytes*/){
	//zero the 2 bytes of checksum
	bzero((void*)(TCP_header + 16),2);
	//compute the remainder including checksum field
	char checksum[2];
	calcChecksum(TCP_header,packetsize,checksum);
	bcopy((void*)checksum,(void*)(TCP_header+16),2);
	/*
	printf("inserted: ");
	printchar(TCP_header[16]);
	printf(" ");
	printchar(TCP_header[17]);
    printf("\n");
	*/
}

int isValidCRC(char* TCP_header,int packetsize){
	char checksum[2];
	char checksumold[2];
	bcopy(TCP_header+16,checksumold,2);
	bzero(TCP_header+16,2);
	calcChecksum(TCP_header,packetsize,checksum);
	bcopy(checksumold,TCP_header+16,2);
	return (checksum[0] == checksumold[0])&&(checksum[1]==checksumold[1]);
}
