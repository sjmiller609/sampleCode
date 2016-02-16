/* Example: client.c sending and receiving datagrams using UDP */
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_MES_LEN 1024

/* client program called with host name and port number of server */
main(int argc, char *argv[])
{
    int sock, buflen;
    struct sockaddr_in name;
    struct hostent *hp, *gethostbyname();

    if(argc < 2) {
	printf("usage: cli remote_port_number\n");
	exit(1);
    }

    /* create socket for connecting to server */
    sock = socket(AF_INET, SOCK_DGRAM,0);
    if(sock < 0) {
	perror("opening datagram socket");
	exit(2);
    }

    /* construct name for connecting to server */
    name.sin_family = AF_INET;
    name.sin_port = htons(atoi(argv[1]));

    /* convert hostname to IP address and enter into name */
    hp = gethostbyname("localhost");
    if(hp == 0) {
		fprintf(stderr, "%s:unknown host\n","localhost"); 
		exit(3);
    }
    bcopy((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);

    /* send message to server */
    uint32_t cli_buf[3];
	int i = 0;
	int dtime;
	int seqnum = 1;
	int portnum;
	for(;i<3;i++){
		dtime = 500 + 1000*i;
		seqnum++;
		cli_buf[0] = htonl(dtime);
		cli_buf[1] = htonl(seqnum);
		cli_buf[2] = htonl(portnum);
		printf("Client sends:    %d msec, %d seqnum, %d portnum\n",cli_buf[0],cli_buf[1],cli_buf[2]);
		if(sendto(sock, (void*) cli_buf, sizeof(uint32_t)*3, 0, (struct sockaddr *)&name, sizeof(name)) <0) {
		perror("sending datagram message");
		exit(4);
		}

		/* waiting for server to respond using the same socket */
		bzero(cli_buf, MAX_MES_LEN);
	}
	fd_set fds;
	int n;
	FD_ZERO(&fds);
	FD_SET(sock,&fds);
    if(recvfrom(sock, cli_buf, buflen, 0) < 0) {
	perror("sending datagram message");
	exit(5);
    }
    printf("Client receives: %s\n",cli_buf);

    /* close connection */
    close(sock);
    exit(0);
}
