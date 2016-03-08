//TCPD.h
//the ftpc or ftps processes will use a UNIX domain socket to communicate with TCPD on the local machine
#define PATH_NAME "tcpd_socket"
struct sockaddr_un tcpd;
int tcpd_len;

/*
int sentTo(char* path_name,int len_path_name){
	if(connect(sock,tcpd,addrlen)){
		close(sock);
		fprintf(stderr,"ERROR: failed to connect to TCPD\n");
		return -1;
	}
	return 0;
}
*/

int SOCKET(int domain, int type, int protocol){
	//TODO: deal with multiple connections
	int sock =  socket(AF_UNIX,SOCK_STEAM,0);
	if(sock < 0){
		fprintf(stderr,"ERROR: failed to create socket to connect to TCPD\n");
		return -1;
	}
	tcpd.sun_family = AF_UNIX;
	strcpy(tcpd.sun_path,PATH_NAME);
	tcpd_len = sizeof(struct sockaddr_un);
	if(connect(sock,tcpd,sizeof(struct sockaddr_un)){
		close(sockfd);
		fprintf(stderr,"ERROR: failed to connect to TCPD\n");
		return -1;
	}
	return sock;
}

int BIND(int sockfd, const struct sockaddr *address, socklen_t address_len){
	/*
	struct sockaddr_un tcpd_bind;
	tcpd_bind.sun_family = AF_UNIX;
	strcpy(tcpd_bind.sun_path,"tcpd_socket_bind");
	if(connect(sockfd,tcpd_bind,sizeof(struct sockaddr_un)){
		close(sockfd);
		fprintf(stderr,"ERROR: failed in BIND wrapper\n");
		return -1;
	}
	int sock =  socket(AF_UNIX,SOCK_STEAM,0);
	write(sockfd,
	*/
	return 0;
}

int LISTEN(int sockfd, int const backlog){
	//return listen(sockfd, backlog);
	return 0;
}

//returns file descriptor of where to read from
int ACCEPT(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen){
	//return accept(sockfd,cliaddr,addrlen);
	return sockfd;
}

//establishes who we are sending to (who is the server)
int CONNECT(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen){
	/*
	//connect with local tcpd
	int result = connect(sockfd,tcpd,addrlen);
	if(!result){
		//send local tcpd the address we are connecting to
		send(sockfd,serv_addr,addrlen,0);
	}
	return result;
	*/
	return 0;
}


ssize_t SEND(int socket, const void *buffer, size_t length, int flags){
	return send(socket,buffer,length,0);
}

ssize_t RECV(int socket, void *buffer, size_t length, int flags){
	return recv(socket,buffer,length,0);
}

int CLOSE(int socket){
	//TODO: close other processes
	return close(socket);
}
