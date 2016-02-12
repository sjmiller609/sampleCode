#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
//#include <arpa/inet.h>
#include <unistd.h>

int errormsg(){
	printf("./ftpc <local file to transfer>\n");
	return -1;
}
//SEND(client_s,out_buf,bytesread,0)
//send to local TCPD process
ssize_t SEND(int sock, const void *buffer,size_t length, int flags,struct sockaddr* nameptr,socklen_t namesize){
	//we are ignoring flags for now
	usleep(10000);
	return sendto(sock,buffer,length,0,nameptr,namesize);
	printf("ftpc sent first %d bytes: of %s\n",length,(char *) buffer);
}
		

int main(int argc,char* argv[]){
	//check number of arguments
	if (argc != 2){
		printf("incorrect number of arguments\n");
		return errormsg();
	}

	long filesize;
	//make sure file exists
	if(access( argv[1], F_OK ) != -1 ) {
		//get size of input file
		if(strlen(argv[1])>20){
			printf("rename <%s> to less than 20 characters\n",argv[1]);
			return errormsg();
		}
		struct stat st;
		stat(argv[1],&st);
		filesize = st.st_size;
	}else{
		printf("can't access file <%s>\n",argv[1]);
		return errormsg();
	}

	//TODO: maybe check valid port num?
	unsigned int		client_s;
	struct sockaddr_in	name;
	struct hostent *hp, *gethostbyname();

	//make UDP socket
	printf("making socket\n");
	if((client_s = socket(AF_INET, SOCK_DGRAM,0))<0){
		fprintf(stderr,"failed to open UDP socket.\n");
		return errormsg();
	}
	//construct name for connect to server
	//AF_NET
	name.sin_family = AF_INET;
	name.sin_port = htons(1337);
	//name.sin_addr.s_addr = inet_addr(argv[1]);
	//send SYN
/*
	if(connect(client_s,(struct sockaddr *)&name, sizeof(name))){
		fprintf(stderr,"failed to connect to server.\n");
		close(client_s);
		return errormsg();
	}
*/

	/* convert hostname to IP address and enter into name */
	// we are sending to troll, located on localhost
	hp = gethostbyname("localhost");
	//!hp TODO??
	if(hp==0) {
		fprintf(stderr, "%s:unknown host\n", argv[1]);
		return errormsg();
	}
	bcopy((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);

	uint32_t netfilesize = htonl(filesize);
	//send filesize
	printf("sending filesize\n");
	if(sizeof(int)!=SEND(client_s,(void*) &netfilesize,sizeof(int),0,(struct sockaddr *)&name,sizeof(name))){
		fprintf(stderr,"failed to send correct file size.\n");
		close(client_s);
		return errormsg();
	}

	//prepare to send 20 bytes for filename
	char filename[20];
	int i;
	for(i=0;i<strlen(argv[1]);i++){
		filename[i] = argv[1][i];
	}
	//pad output buffer to 20 bytes
	while(i<20){
		filename[i] = '\0';
		i++;
	}
	//send 20 bytes for filename
	printf("sending filename\n");
	if(20!=SEND(client_s,filename,sizeof(filename),0,(struct sockaddr *)&name,sizeof(name))){
		fprintf(stderr,"ERROR: sent wrong number of bytes for file name.\n");
		close(client_s);
		return errormsg();
	}
	//open file for reading
	FILE* fp = fopen(filename,"rb");
	//make sure it opened
	if(fp==NULL){
		printf("could not open file\n");
		close(client_s);
		return errormsg();
	}
	char out_buf[1000];
	int bytesread = 0;
	long fileleft = filesize;
	//send file, 1000 bytes sizeat a time
	printf("sending file\n");
	while(fileleft){
		//read up to 1000 bytes into out_buf
		bytesread = fread((void*) out_buf,sizeof(char),sizeof(out_buf)/sizeof(char),fp);
		//send out_buf to server, make sure all bytes were sent
		if(bytesread!=SEND(client_s,out_buf,bytesread,0,(struct sockaddr *)&name,sizeof(name))){
			fprintf(stderr,"Oops! didn't send all bytes, %ld bytes were left in file\n", fileleft);
			fclose(fp);
			close(client_s);
			return errormsg();
		}
		//decrement file left
		fileleft-=bytesread;
	}
	fflush(fp);
	fclose(fp);
	close(client_s);
	return 0;
}
