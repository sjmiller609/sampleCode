#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>

int errormsg(){
	printf("./ftps\n");
	return -1;
}

ssize_t RECV(int socket, void *buffer, size_t length, int flags,struct sockaddr *address,int* address_len){
	//let tcpd know we are waiting, and also how many bytes to send max
	char recvmaxandflags[2*sizeof(uint32_t)];
	uint32_t lengthnet = htonl(length);
	uint32_t flagsnet = htonl(flags);

	bcopy(&lengthnet,recvmaxandflags,sizeof(uint32_t));
	bcopy(&flagsnet,recvmaxandflags+sizeof(uint32_t),sizeof(uint32_t));

	if(sendto(socket,(void *) recvmaxandflags,sizeof(uint32_t)*2,0,address,*address_len)<0){
		errormsg();
		exit(2);
	}
	return recvfrom(socket,buffer,length,0,address,address_len);
}

int main (int argc, char *argv[]){

	int	 		sock;
	struct sockaddr_in 	name;
	//unsigned int 		connect_s;
	struct sockaddr_in	client_addr;
	struct in_addr		client_ip_addr;
	int			addr_len;
	int namelen;
	struct hostent *hp, *gethostbyname();

	//check number of arguments
	/*
	if (argc != 1){
		fprintf(stderr,"incorrect number of arguments\n");
		return errormsg();
	}
*/


/*
	//assign localport
	int localport = atoi(argv[1]);
	printf("server assigned port %d\n",localport);
*/

	//make server
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("could not open socket.\n");
		return errormsg();
	}

	//put together sockaddr_in struct
	name.sin_family = AF_INET;
	name.sin_port   = htons(1337);
	hp = gethostbyname("localhost");
    	bcopy((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);
/*	
	//bind sock to socket address
	if(bind(sock, (struct sockaddr *)&name,sizeof(name))<0){
		printf("could not bind socket to port\n");
		return errormsg();
	}
*/
    	namelen=sizeof(name);
/*
	//start listening
	listen(sock, 1);

	//block
	connect_s = accept(sock, (struct sockaddr *)&client_addr,&addr_len);

	//get client data
	memcpy(&client_ip_addr,&client_addr.sin_addr.s_addr,4);
	
	//report client IP and port num to server stdout
	printf("Accept completed!\nclient IP: %s\nclient port: %d\n",inet_ntoa(client_ip_addr),ntohs(client_addr.sin_port));
	
*/
	//blocking recv filesize
	int filesize;
	//4 bytes
	uint32_t netfilesize;
	char filename[20];
	//recv filesize
	if(sizeof(int)!=RECV(sock,(void*)&netfilesize,sizeof(uint32_t),MSG_WAITALL,(struct sockaddr *)&name,&namelen)){
		printf("file transfer failed\n");
		return errormsg();
	}

	//host bit order
	filesize = ntohl(netfilesize);

	//blocking recv file name
	printf("filesize = %d\n",filesize);
	if(20!=RECV(sock,filename,20,MSG_WAITALL,(struct sockaddr *)&name,&namelen)){
		printf("file transfer failed\n");
		return errormsg();
	}
	//report filename to stdout
	printf("filename = %s\n",filename);
	
	//make ./recv directory with appropriate permissions
	mkdir("./recv", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	//allocate space for string of path
	char path[strlen("./recv/")+strlen(filename)+1];
	strcpy(path,"./recv/");
	strcat(path,filename);

	//open file for writing in binary mode in subdirectory
	FILE* fp = fopen(path,"wb");
	//check that the file opened
	if(fp==NULL){
		//close(connect_s);
		close(sock);
		fprintf(stderr,"failed to open file for writing\n");
		return errormsg();
	}
	//report where we are writing to
	printf("writing to <%s>\n",path);

	//prepare to recv file
	int recved = 0;
	char in_buf[1000];
	long fileleft = filesize;
	int last = 0;
	int written = 0;
	//needs to be long so there isn't overflow when mult by 100
	long totalwrite = 0;
	while(fileleft > 0){
		recved = RECV(sock,in_buf,sizeof(in_buf),0,(struct sockaddr *)&name,&namelen);
		written = fwrite((void*) in_buf,sizeof(char),recved/sizeof(char),fp);
		totalwrite+=written;
		//make sure that all bytes recv'd were written to the file
		if(written != recved){
			fprintf(stderr,"ERROR: did not write all bytes, %ld bytes remaining\n",fileleft);
			fclose(fp);
			//close(connect_s);
			close(sock);
			return errormsg();
		}
		fileleft -= recved;
		//show % done if greater than last time, /100s are to dodge overflow
		/*
		if(last < totalwrite*100/filesize){
			last = totalwrite*100/filesize;
			printf("%d%\n",last);
		}
		*/
		printf("file left = %d bytes\n",fileleft);
	}
	//make sure all of the file is persisted
	printf("flushing buffer...\n");
	//fflush(fp);
	fclose(fp);
	//close(connect_s);
	close(sock);
	printf("%d bytes writtten to %s\n",totalwrite,filename);
	return 0;
}
