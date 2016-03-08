#include <unistd.h>
#include <semaphore.h>

#define MSS 1000
#define BOXSIZE 64000 //64 kB
#define WINDOWSIZE 20

char* outbox;
int outboxWritten;
int outboxRead;
sem_t outboxToRead;
sem_t outboxSpaceLeft;

#define OUTBOXCOUNT (outboxWritten - outboxRead)

char* inbox;
int inboxWritten;
int inboxRead;
sem_t inboxToRead;
sem_t inboxSpaceLeft;


#define INBOXCOUNT (inboxWritten - inboxRead)
#define TIMERPORT "5421"

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

void startTimer(){
	if(fork()){
		execv("./timer-process",TIMERPORT);
		fprintf(stderr,"ERROR: this should be unreachable code\n");
	}
	return;
}

int initialize(){

	startTimer();
	inbox = malloc(BOXSIZE);
	outbox = malloc(BOXSIZE);
	if(inbox == NULL || outbox == NULL){
		fprintf(stderr,"ERROR: failed to initialize buffers in TCPD\n");
		return -1;
	}
	outboxWritten = 0;
	outboxRead = 0;
	inboxWritten = 0;
	inboxRead = 0;
	sem_init(outboxToRead,0,0);
	sem_init(outboxSpaceLeft,0,BOXSIZE);
	sem_init(inboxToRead,0,0);
	sem_init(inboxSpaceLeft,0,BOXSIZE);
	return 0;
}

char* outboxAddressAtIndex(int index){
	return outbox + (index % BOXSIZE);
}
char* inboxAddressAtIndex(int index){
	return inbox + (index % BOXSIZE);
}

void* outboxProducer(void* args){
	int msgSock = *((int*) args);
	int nextWrite = 0;
	int recvd;
	while(1){
	//TODO: termination logic
		sem_wait(outboxSpaceLeft);
		recvd = recv(msgSock,outboxAddressAtIndex(nextWrite),1,MSG_WAITALL);
		//TODO: deal with overflow of nextWrite
		nextWrite++;
		if(recvd != 1){
			fprintf(stderr,"failed to recv outboxProducer\n");
			return;
		}
		sem_post(outboxToRead);
	}
}

int claimUpTo(sem_t* semaphore,int maxclaim){
	//TODO: make non iterative to reduce computational load
	//calculate instead
	int i = 0;
	for(;i < maxclaim ;i++){
		if(sem_trywait(*semaphore)){
			return i;
		}
	}
	return i;
}

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

char* window[20];
void loadWindow(char* buffer, int buffersize,int index){
	static int seqnum = -1;
	seqnum++;
	char* packet = malloc(buffersize+sizeof(struct tcpheader));
	//TODO: what if malloc fails
	struct tcpheader* headerPtr = (struct tcpheader*) packet;
	char* bodyPtr = tcpheader+sizeof(struct tcpheader);
	headerPtr->sourceport = htons(SOURCE_PORT);
	headerPtr->destport = htons(DEST_PORT);
	headerPtr->seqnum = htonl(seqnum);
	headerPtr->acknum = htonl();
	headerPtr->dataoff_res_flags = htons();
	headerPtr->windowsize = htons(20);
	headerPtr->checksum = htons();
	//check for if the buffer is split or not (circular array)
	int splitCheck = BOXSIZE - (buffer - outbox) - buffersize;
	if(splitCheck < 0){
		//buffer is split
		splitCheck = 0 - splitCheck;
		bcopy(buffer,bodyPtr,buffersize - splitCheck);
		bcopy(outbox,bodyPtr+(buffersize - splitCheck),
			splitCheck - 1);
	}else{
		//buffer is contiguous
		bcopy(buffer,bodyPtr,buffersize);
	}
	window[index] = packet;
}

//send from outbox to remote and timer-process, also deal with retransmission
void* outboxConsumer(void* args){
/*
//TODO: retransmission
	int fromTimerSock, fromTimerAddrLen;
	struct sockaddr_in fromTimerAddr;
	int listenport;
	fromTimerSock = socket(AF_INET,SOCK_DGRAM,0);
	if(fromTimerSock < 0) {
		perror("opening datagram socket");
		exit(1);
	}
	fromTimerAddr.sin_family = AF_INET;
	fromTimerAddr.sin_port = 0;
	fromTimerAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	fromTimerAddrLen = sizeof(struct sockaddr_in);
	if(bind(fromTimerSock,(struct sockaddr*)&fromTimerAddr,
		fromTimerAddrLen)<0){
		perror("error binding to socket");
		exit(2);
	}
    if(getsockname(fromTimerSock, (struct sockaddr *)&fromTimerAddr,&fromTimerAddrLen) < 0){
		perror("getting sock name");
		exit(3);
    }
	listenport = ntohs(fromTimerAddr.sin_port);
	fromTimerAddrLen = sizeof(struct sockaddr_in);
*/
	
	int nextConsumeIndex = 0;
	int claimed;
	int i;
	int extraPacket = 0;//boolean
	//TODO: stop spinning when nothing to claim (delay or something)
	while(1){
	//TODO: termination logic
		//claim up to max window size in the outbox
		//"claimed" will hold the number of bytes claimed
		claimed = claimUpTo(&outboxToRead,MSS*WINDOW);
		//we should calculate how many packets we will be sending
		numPackets = claimed/WINDOW;
		//need to send one packet < MSS if claimed/WINDOW has remainder
		extraPacket = claimed % WINDOW;//boolean
		for(i=0;i<numPackets;i++){
			loadWindow(outboxAddressAtIndex(nextConsumeIndex),MSS);
			nextConsumeIndex += MSS;
		}
		if(extraPacket){
			loadWindow(outboxAddressAtIndex(nextConsumeIndex),
				claimed - MSS*(numPackets));
			nextConsumeIndex += claimed - MSS*(numPackets);
		}
		for(;claimed>0;claimed--){
			sem_post(outboxSpaceLeft);
		}
	}
}

void* inboxProducer(void* args){
	int remoteSock = *((int*) args);
	int nextWrite = 0;
	int recvd;
	while(1){
		//TODO: termination logic
		sem_wait(inboxSpaceLeft);
		recvd = recv(remoteSock,inboxAddressAtIndex(nextWrite),1,MSG_WAITALL);
		//TODO: deal with overflow of nextWrite
		nextWrite++;
		if(recvd != 1){
			fprintf(stderr,"failed to recv inboxProducer\n");
			return;
		}
		sem_post(inboxToRead);
	}
}

void* inboxConsumer(void* args){

}

int main(int argc, char** argv){

	initialize();
	//listen on tcpd_socket unix domain socket for a connection
	//connect to local process
	int listenSock, msgSock;
	struct sockaddr_un server,client;
	listenSock = socket(AF_INET,SOCK_STREAM,0);
	if(listenSock <0){
		fprintf(stderr,"could not make IPC socket\n");
		return -1;
	}
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path,"tcpd_socket");
	if(bind(listenSock,(struct sockaddr*) &server, sizeof(struct sockaddr_un))){
		fprintf(stderr,"could not bind IPC socket\n");
		return -1;
	}
	listen(listenSock,1);
	msgSock = accept(listenSock,client,sizeof(struct sockaddr_un));

	int i;
	int numThreads = 4;
	pthread_t threads[numThreads];

	//thread 0: waits on msgSock, copies into outbox
	if(pthread_create(&threads[i], NULL, &outboxProducer,(void*) &msgSock)){
		printf("unable to create thread outboxProducer\n");
		return -1;
	}
	i++;

	//thread 1: sends packets to remote and timer-process from outbox
	//also responsible for retransmission
	if(pthread_create(&threads[i], NULL, &outboxConsumer,(void*)NULL)){
		printf("unable to create thread outboxProducer\n");
		return -1;
	}
	i++;

	//thread 2: waits on remotesock, strips headers and copies to inbox
	//also responsible for retransmission
	if(pthread_create(&threads[i], NULL, &inboxProducer,(void*)NULL)){
		printf("unable to create thread outboxProducer\n");
		return -1;
	}
	i++;

	//thread 3: consumes inbox and sends to main process
	//also responsible for retransmission
	if(pthread_create(&threads[i], NULL, &inboxConsumer,(void*)NULL)){
		printf("unable to create thread outboxProducer\n");
		return -1;
	}

	/*waiting for threads to be done*/
	for(i = 0; i < numThreads; i++){
		if(pthread_join(threads[i], NULL)){
			printf("unable to close thread %d\n", i);
		}
	}

	close(listenSock);
	close(msgSock);
	free(outbox);
	free(inbox);
	return 0;
}
