#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "troll.h"

int errormsg(){
	printf("./tcpd <server IP OR / \"server\">\n");
	return -1;
}

int main(int argc,char* argv[]){

	if(argc != 2){
		fprintf(stderr,"too few args\n");
		return errormsg();
	}
	int sock, namelen, buflen;
	char srv_buf[1016];
	struct sockaddr_in name, dest;

	/*create socket*/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("opening datagram socket");
		exit(1);
	}

	/* create name with parameters and bind name to socket */
	name.sin_family = AF_INET;
	name.sin_port = htons(1337);
	name.sin_addr.s_addr = htons(INADDR_ANY);
	if(bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
		perror("getting socket name\n");
		exit(2);
	}

	dest.sin_family = AF_INET;
	dest.sin_port = htons(1338);
	dest.sin_addr.s_addr = htons(INADDR_ANY);

	/* waiting for connection from client on name and print what client sends */
	namelen = sizeof(name);
	buflen = 1000;
	uint32_t maxbytes;
	uint32_t flags;
	int i;
//=====================
	int sock_remote;

	/*create socket*/
	sock_remote = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_remote < 0) {
		perror("opening datagram socket");
		exit(1);
	}

	/* create name with parameters and bind name to socket */
	name.sin_port = htons(1338);
	if(!strcmp(argv[1],"server")){
		if(bind(sock_remote, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
			perror("getting socket name for remote\n");
			exit(2);
		}
	}

	int recvd;
	int sent;
	int destlen;
	//dest = remote troll, name = ftps
	while(!strcmp(argv[1],"server")){
		//wait for ftps to say it's ready for a packet
		recvd = recvfrom(sock, srv_buf, buflen, 0,
		 (struct sockaddr *)&name, &namelen);
		if(recvd < 0) {
			perror("error receiving"); 
			exit(4);
		}

		//what size packet does it want?
		bcopy(srv_buf,&maxbytes,sizeof(uint32_t));
		//what is the flag?
		bcopy(srv_buf+sizeof(uint32_t),&flags,sizeof(uint32_t));
		printf("max bytes = %d, flags = %d\n",ntohl(maxbytes),ntohl(flags));

		bzero(srv_buf,sizeof(srv_buf));
		recvd = recvfrom(sock_remote, srv_buf, ntohl(maxbytes)+16,
			 0,(struct sockaddr *)&dest,&destlen);
		if(recvd < 0){
			perror("error receiving"); 
			exit(4);
		}else{
			printf("recv'd %d bytes from remote host\n",recvd);
		}
		
		//put message from remote tcpd into srv_buf
		sent = sendto(sock, srv_buf+16, recvd-16, 0,
		 (struct sockaddr *)&name, namelen);

		if(sent < 0) {
			printf("failed to send data to ftps, sent = %d\n",sent);
			perror("sending datagram");
			exit(5);
		}else{
			printf("tcpd sent %d bytes to ftps\n",sent);
		}
		if((flags==MSG_WAITALL) && (sent != ntohl(maxbytes))){
			fprintf(stderr,"did not send enough bytes. %d sent\n",sent);
			return -1;
		}
	}

	struct sockaddr_in troll;

	troll.sin_family = AF_INET;
	troll.sin_port = htons(1338);

    /* convert hostname to IP address and enter into name */
    struct hostent *hp;
    hp = gethostbyname("localhost");
    if(hp == 0) {
	fprintf(stderr, "%s:unknown host\n", argv[1]);
	exit(3);
    }
    bcopy((char *)hp->h_addr, (char *)&troll.sin_addr, hp->h_length);


	char for_troll[1016];
	while(1){

		//forward all from ftpc (1337) to troll (on 1338)
		// get packet from ftpc
		recvd = recvfrom(sock, srv_buf, sizeof(srv_buf), 0,
		 (struct sockaddr *)&name, &namelen);
		if(recvd < 0){
			perror("error receiving from ftpc"); 
			exit(4);
		}else{
			printf("tcpd recv'd %d bytes from ftpc\n",recvd);
		}

		//make new NetMessage
		struct sockaddr_in header;

		header.sin_family = htons(AF_INET);
		header.sin_addr.s_addr = inet_addr(argv[1]);
		header.sin_port = htons(1338);

		//add 16 byte header and pack contents to for_troll
		bcopy(&header,for_troll,sizeof(header));
		bcopy(srv_buf,for_troll+sizeof(header),recvd);

		/*
		printf("srv_buf = %s\n",srv_buf);
		printf("message.body = %s\n",message.body);
		*/

		sent = sendto(sock_remote, for_troll, recvd+16, 0,
		 (struct sockaddr *)&troll,sizeof(troll));

		if(sent < 0) {
			perror("failed to send to troll");
			exit(5);
		}else{
			printf("tcpd sent %d bytes to troll\n",sent);
		}
	}

}
