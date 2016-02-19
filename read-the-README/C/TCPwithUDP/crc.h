//header file to implement the CRC functions.
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>

void shift_right(char *ar, int size,int start)
{
	/*
	char* c = &(ar[start/8]);
	uint32_t* p32 = (uint32_t*)c;
	*p32 = htonl((ntohl(*p32) >> 1));
	*/
	*((uint32_t*)&(ar[start/8])) = 
		htonl((ntohl(*((uint32_t*)&(ar[start/8])))>>1));
}   

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

	for(;i<packetsize*8;i++){
		if(
			working_buffer[i/8]
				&
			(1<<(7-i%8))
		){//if bit i is 1
			for(j=i/8;j<(i/8+3);j++){
				working_buffer[j] = working_buffer[j]^poly[j];
			}
		}
		shift_right(poly,packetsize+2,i);
	}
	bcopy((void*)(working_buffer+packetsize),(void*)checksum,2);
}

//CRC-16
void insertChecksum(char* TCP_header,int packetsize/*bytes*/){
	//zero the 2 bytes of checksum
	bzero((void*)(TCP_header + 16),2);
	//compute the remainder including checksum field
	char checksum[2];
	calcChecksum(TCP_header,packetsize,checksum);
	bcopy((void*)checksum,(void*)(TCP_header+16),2);
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
