//DOUBLE LINKED LIST LIST IMPLEMENTATION OF DELTA TIMER
//Delta-list implementaion for timer this process receives on a port a packet indicating amount of time and a port to respond to
//incoming packet UDP data: time (32 bits), seqnum (32 bits), port num to respond to (32 bits)
//send packet data: seqnum (32 bits)
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

struct Dnode{
	int dtime;
	int seqnum;
	int portnum;
	struct Dnode* next;
	struct Dnode* prev;
};

//pointers to the two dummy nodes at front / back of doubly linked list
struct Dnode* front;
struct Dnode* back;

//storage for rest of nodes
struct Dnode* nodes;
int sizeofnodes;
int nodecount;

int insert(struct Dnode* insertme){
	nodecount++;
 	//table doubling
	if(nodecount*sizeof(struct Dnode) > sizeofnodes){
		//TODO deal with failure
		nodes = realloc(nodes,sizeofnodes*2);
		sizeofnodes = sizeofnodes*2;
	}
	//copy temporary values into the Node stack
	nodes[nodecount-1].dtime = insertme->dtime;
	nodes[nodecount-1].seqnum = insertme->seqnum;
	nodes[nodecount-1].portnum = insertme->portnum;
	//change pointer to new memory
	insertme = &(nodes[nodecount-1]);
	//get a pointer to the first node in the list (not the front dummy node)
	struct Dnode* current = front->next;
	//compare current pointer to node to the back ndoe to make sure we aren't at end of list
	while(current != back){
		if(insertme->dtime > current->dtime){
			insertme->dtime -= current->dtime;
			current = current->next;
		}else {/*note: for current->dtime == insertme->dtime,
			we will insert and make the current node dtime = zero,
			which will matter later*/
			/*subtract current d time on insert from the 
			node which will be behind our new node*/
			current->dtime -= insertme->dtime;
			//set up pointers in "insertme"
			insertme->next = current;
			insertme->prev = current->prev;
			//insert
			insertme->next->prev = insertme;
			insertme->prev->next = insertme;
		}
	}
	//we are at the end of the list, since we escaped the while loop
	//set up pointers in "insertme"
	insertme->next = back;
	insertme->prev = back->prev;
	//insert
	insertme->next->prev = insertme;
	insertme->prev->next = insertme;
}

int outsock;
int sendmessage(int seqnum, int portnum){
	struct sockaddr_in dest;
	uint32_t outint = htonl(seqnum);
	dest.sin_family = AF_INET;
	dest.sin_port = htons(portnum);
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	return sendto(outsock,&outint,sizeof(outint),0,
			(struct sockaddr*)&dest,sizeof(dest));
}

//removes the expired node from doubly linked list and stack
int timerbuzz(){
	//pointer to mem location of removing node
	struct Dnode* vacantPtr = front->next;
	//send buzzer message
	if(sendmessage(vacantPtr->seqnum,vacantPtr->portnum)){
		fprintf(stderr,"failed to send buzzer message\n");
		return -1;
	}
	//remove node from dll
	front->next->next->prev = front;
	front->next = front->next->next;
	//copy data from last node in stack into vacant mem location
	*vacantPtr = nodes[nodecount-1];
	//swap pointers to new location
	nodes[nodecount-1].next->prev = vacantPtr;
	nodes[nodecount-1].prev->next = vacantPtr;
	//indicate node removed in node stack
	nodecount--;
	//potentially free some space in the nodes stack
	if(nodecount*sizeof(struct Dnode)*2 < sizeofnodes){
		nodes = realloc(nodes,sizeofnodes/2);
		sizeofnodes = sizeofnodes/2;
	}
	return 0;
}
//returns -1 if invalid port number
//otherwise returns the port number
int portNum(char port[]){

	//printf("checking if %s is valid port num\n",port);
	int i = 0;
	//checking for negative numbers
	if (port[0] == '-') return -1;
	for (; port[i] != '\0'; i++){
		if (!isdigit(port[i])) return -1;
	}
	i = atoi(port);
	if(i < 1024 || i > 65525) return -1;
	//printf("returning %d\n",i);
	return i;
}

void print_bytes(const void *object, size_t size)
{
  size_t i;

  printf("[ ");
  for(i = 0; i < size; i++)
  {
    printf("%02x ", ((const unsigned char *) object)[i] & 0xff);
  }
  printf("]\n");
}
int main(int argc, char* argv[]){
	//socket to output with
	outsock = socket(AF_INET,SOCK_DGRAM,0);
	int port = portNum(argv[1]);
	//get port from input, make sure it's valid and suck
	if(argc < 2 || port<0 ){
		fprintf(stderr,
		"timer-process failed to start: invalid port number\n");
		return -1;
	}
	//intitialize doubly linked list to implement delta-timer
	//using two dummy nodes
	nodes = (struct Dnode*) malloc(2*sizeof(struct Dnode));
	sizeofnodes = 2*sizeof(struct Dnode);
	nodecount = 2;
	front = &(nodes[0]);
	back = &(nodes[1]);
	front->next = back;
	front->prev = NULL;
	back->prev = front;
	back->next = NULL;
	back->dtime = INT_MAX;

	int sock;
	struct sockaddr_in name;

	/*create socket*/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("opening datagram socket");
		exit(1);
	}

	/* create name with parameters and bind name to socket */
	name.sin_family = AF_INET;
	printf("port = %d\n",port);
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
		perror("error binding socket\n");
		exit(2);
	}

	fd_set fds;
	int n;
	struct timeval waittime;
	uint32_t buffer[3];

	// Set up the file descriptor set.
	FD_ZERO(&fds);
	//add "sock" to reading set for select
	FD_SET(sock, &fds);

	while(1){
		// Set up the struct timeval for the timeout.
		waittime.tv_sec = (front->next->dtime)/1000;
		//set up wait to be the amout of time until the next node should be expired
		waittime.tv_usec = (front->next->dtime-waittime.tv_sec*1000)*10;
		// Wait until timeout or data received.
		printf("waiting on port %d\n",ntohs(name.sin_port));
		n = select(sock+1, &fds, NULL, NULL, &waittime);
		printf("select returned %d\n",n);
		if (n == 0 && front->next != back){//timeout
				if(timerbuzz()){
					fprintf(stderr,"timerbuzz failed\n");
					return -1;
				}

		}else if(n == -1){//error
			printf("select returned -1\n");
			return 1;   
	
		}else{//ready to read
			//decrement front to be appropriate amount of time left
			front->next->dtime -= waittime.tv_sec*1000 - waittime.tv_usec/10;
			if(front->next->dtime < 0){
				fprintf(stderr,"oops! subracted too much time\n");
				return -1;
			}
			int recvd = 0;
			recvd = recvfrom(sock,(void*)buffer, sizeof(uint32_t)*3, 0, NULL, NULL);
			print_bytes(buffer,sizeof(buffer));
			if (recvd != sizeof(uint32_t)*3){
				fprintf(stderr,"did not recv amout of bytes expected.\n");
				return -1;
			}
			//buffer = <time,seqnum,respond_port> (network byte order)
			struct Dnode new;
			new.dtime = ntohl(buffer[0]);
			new.seqnum = ntohl(buffer[1]);
			new.portnum = ntohl(buffer[2]);
			printf("received %d msec,%d seqnum,%d portnum\n",
					new.dtime, new.seqnum, new.portnum);

			//check for exit
			if(new.dtime < 0){
				fprintf(stderr,"negative number to wait, exiting\n");
				free(nodes);
				return 0;
			}
			//insert
			if(insert(&new)){
				fprintf(stderr,"failed to insert new node\n");
				return -1;
			}
	
		}
	}
}
