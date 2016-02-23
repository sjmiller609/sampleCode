#define MAX_MSS 1000

typedef struct tcpheader{
	uint16_t sourceport;
	uint16_t destport;
	uint32_t seqnum;
	uint32_t acknum;
	uint16_t dataoff_res_flags;
	uint16_t windowsize;
	uint16_t checksum;
	uint16_t urgentptr;
	uint32_t optionsandpadding;
}tcpheader;

struct listenQ{
	tcpheader header;
	int state;
	struct sockaddr cliaddr;
	int cliaddrlen;
	struct listenQ* next;
};
struct listenQ* front;
struct listenQ* back;
int LQbacklog;
pthread_mutex_t listenQmutex;

int SOCKET(int domain, int type, int protocol){
	//only ok to use AF_INET sockets for this implementation
	//just using switch so it's easier to expand later.
	switch(domain) {
	   case AF_INET:
	      //this is what we are overriding, break to proceed
	      	break;
	   default :
		fprintf(stderr,"only AF_INET domain is implemented.\n");
	   return -1;
	}
	//only ok to use AF_INET sockets for this implementation
	switch(type) {
	   case SOCK_STREAM:
	      //this is the custom TCP option
	      //break to proceed
	      break;
	/* //Don't want to allow this for now
	   case SOCK_DGRAM:
	      //just asking for normal UDP
	      return socket(domain,type,protocol);
	*/
	   default :
		   fprintf(stderr,"this type is not implemented for SOCKET\n");
	   return -1;
	}
	return socket(domain,SOCKET_DGRAM,0);
}

//assigns socket to an address
int BIND(int sockfd, const struct sockaddr *address, socklen_t address_len){
	return bind(sockfd,address,address_len);
}

//prepare it for incoming connections
//set up recv buffer stuff
//listen queue of max length @backlog
//passive open, waits for client SYN
void* listenfunc(void** args){
	struct sockaddr cliaddr;
	char[sizeof(tcpheader)+MAX_MSS] buffer;
	int recvd;
	for(;;){
		//this socket should only pay attention to valid SYN packets
		recvd = recvfrom(sockfd, buffer, sizeof(buffer)
		,0,cliaddr,sizeof(cliaddr));
		if(isValidSYN(&buffer,recvd)){
			enqueue(&buffer,sizeof(buffer));
		}else if(isValidFIN(&buffer,recvd)){
			finRecvd();
			return NULL;
		}
		bzero(buffer,recvd);
	}
}
/*
struct Listenq{
	tcpheader header;
	int state;
	struct Listenq* nextptr;
};*/
bool listening = false;
int LISTEN(int sockfd, int const backlog){
	if(listening){
		fprintf(stderr,"error: already listening\n");
		return -1;
	}
	listening = true;
	LQbacklog = backlog;
	if(pthread_mutex_init(&listenqmutex,NULL)){
		fprintf(stderr,"failed to initialize listenqmutex\n");
		return -1;
	}
	//create detached thread
	pthread_t asynclisten;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachedstate(&attr,PTHREAD_CREATE_DETACHED);
	if(pthread_create(&asynclisten,&attr,&listenfunc,NULL)){
		fprintf(stderr,"failed to create listening thread\n");
		return -1;
	}
	return 0;
}

//creates a new socket for each connection and removes the connection from the listen queue. returns new socket descriptor of accepted connection
//deals with SYNs in the listen queue
int ACCEPT(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen){
	
}

int CONNECT(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen){

	return 0;
}

ssize_t SEND(int socket, const void *buffer, size_t length, int flags){

}

ssize_t RECV(int socket, void *buffer, size_t length, int flags){


}

int CLOSE(int socket){

}
