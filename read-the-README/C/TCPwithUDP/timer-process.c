//DOUBLE LINKED LIST LIST IMPLEMENTATION OF DELTA TIMER
//Delta-list implementaion for timer this process receives on a port a packet indicating amount of time and a port to respond to
//incoming packet UDP data: time (32 bits), seqnum (32 bits), port num to respond to (32 bits)
//send packet data: seqnum (32 bits)
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

struct Dnode{
	int dtime;
	int seqnum;
	int portnum;
	struct Dnode* next;
	struct Dnode* prev;
};
int verbose = 0;

//pointers to the two dummy nodes at front / back of doubly linked list
struct Dnode* front;
struct Dnode* back;

//storage for rest of nodes
int nodecount;

int print_dtime(){
	struct Dnode* current;
	current = front->next;
	printf("\t");
	int i = 2;
	while(current != back){
		if(current->next == current){
			fprintf(stderr,"seqnum %d next is itself\n");
			return -1;
		}
		
		printf("[dtime: %d, seqnum: %d],",current->dtime,current->seqnum);
		current = current->next;
		i++;
	}
	printf("%d\n",current->dtime);
	if(i != nodecount){
		fprintf(stderr,"counted %d elements, but should be %d\n",i,nodecount);
		return -1;
	}
	return 0;
}

//subtract milliseconds from the timer list
int subtractTime(int msec){
	struct Dnode* current = front->next;
	while(msec>0){
		if(current!=back){
			if(msec > current->dtime){
				msec -= current->dtime;
				current->dtime = 0;
				current = current->next;
			}else{
				current->dtime -= msec;
				msec = 0;
			}
		}else{
			msec = 0;
		}
	}
	return 0;
}
int insert(struct Dnode* insertme){
	if(verbose){
		printf("\tinserting node, dtime order before:\n");
		if(print_dtime()) return -1;
	}
	//get a pointer to the first node in the list (not the front dummy node)
	struct Dnode* current = front->next;
	//compare current pointer to node to the back node to make sure we aren't at end of list
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
			nodecount++;

			if(verbose){
				printf("\torder after insert\n");
				if(print_dtime()) return -1;
			}
			return 0;
		}
	}
	//we are at the end of the list, since we escaped the while loop
	//set up pointers in "insertme"
	insertme->next = back;
	insertme->prev = back->prev;
	//insert
	insertme->next->prev = insertme;
	insertme->prev->next = insertme;
	nodecount++;

	if(verbose){
		printf("\torder after insert\n");
		if(print_dtime()) return -1;
	}
	return 0;
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
int removeExpired(){
	if(verbose){
		printf("\tdeleting node, order before:\n");
		if(print_dtime()) return -1;
	}
	if(front->next->dtime < 0){
		fprintf(stderr,"oops! a node has negative time: %d\n",front->next->dtime);
		return -1;
	}
	while(!(front->next->dtime)){
		//pointer to mem location of removing node
		struct Dnode* remove_me = front->next;
		//send buzzer message
		if(sendmessage(remove_me->seqnum,remove_me->portnum)!=sizeof(uint32_t)){
			fprintf(stderr,"failed to send buzzer message\n");
			return -1;
		}
		//remove node from dll
		remove_me->next->prev = remove_me->prev;
		remove_me->prev->next = remove_me->next;

		free(remove_me);
		//indicate node removed in node stack
		nodecount--;
	}
	return 0;
}

void deleteNode(int seqnum){
	struct Dnode* current = front->next;
	//struct Dnode* last_inserted = &(nodes[nodecount -1]);
	while(current!=back){
		if(current->seqnum == seqnum){
			if(verbose){
				printf("deleting: dtime = %d, seqnum = %d\n",
				current->dtime,current->seqnum);
			}
			current->prev->next = current->next;
			current->next->prev = current->prev;
			if(current->next != back)
			current->next->dtime += current->dtime;
			free(current);
			nodecount--;
			break;
		}
		current = current->next;
	}
}

//returns -1 if invalid port number
//otherwise returns the port number
int portNum(char port[]){

	int i = 0;
	//checking for negative numbers
	if (port[0] == '-') return -1;
	for (; port[i] != '\0'; i++){
		if (!isdigit(port[i])) return -1;
	}
	i = atoi(port);
	if(i < 1024 || i > 65525) return -1;
	return i;
}

int freeNodes(){
	int i = 0;
	struct Dnode* current = front;
	while(current != back){
		current = current->next;
		free((void *) current->prev);
	}
	//free last node (back)
	free(current);
	return 0;
}
int main(int argc, char* argv[]){
	if(argc < 2){
		fprintf(stderr,
		"%s <port number to listen>\n",argv[0]);
		return -1;
	}
	int port = portNum(argv[1]);
	if(port<0){
		fprintf(stderr,
		"timer-process failed to start: invalid port number\n");
		return -1;
	}
	if(argc == 3 &&
	 (!strcmp(argv[2],"-v") || !strcmp(argv[2],"--verbose"))
	){
		printf("using verbose mode.\n");
		verbose = 1;
	}
	//socket to output with
	outsock = socket(AF_INET,SOCK_DGRAM,0);
	//intitialize doubly linked list to implement delta-timer
	//using two dummy nodes
	front = (struct Dnode*) malloc(sizeof(struct Dnode));
	back = (struct Dnode*) malloc(sizeof(struct Dnode));
	front->next = back;
	front->prev = NULL;
	back->prev = front;
	back->next = NULL;
	back->dtime = INT_MAX;
	nodecount = 2;

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

	uint64_t diff;
	struct timespec start,end;
	clock_gettime(CLOCK_REALTIME,&start);
	while(1){

		// Set up the file descriptor set.
		FD_ZERO(&fds);
		//add "sock" to reading set for select
		FD_SET(sock, &fds);
		
		// Wait until timeout or data received.
		clock_gettime(CLOCK_REALTIME,&end);
		diff = 1000000000L*(end.tv_sec-start.tv_sec)+end.tv_nsec-start.tv_nsec;
		subtractTime((int)(diff/1000000));
		// Set up the struct timeval for the timeout.
		waittime.tv_sec = (front->next->dtime)/1000;
		waittime.tv_usec = (front->next->dtime-waittime.tv_sec*1000)*1000;
		if(verbose) printf("\nfront->next->dtime time is %d msec, waiting...\n",
				front->next->dtime);
		n = select(sock+1, &fds, NULL, NULL, &waittime);
		clock_gettime(CLOCK_REALTIME,&start);

		if (n == 0 && front->next != back){//timeout
				if(verbose) printf(
				"select timed out: subtracting time from list\n");
				front->next->dtime = 0;
				if(removeExpired()){
					fprintf(stderr,"failed to remove expired\n");
					return -1;
				}
		}else if(n == -1){//error
			printf("select returned -1\n");
			return 1;   
	
		}else{//ready to read: inserting or deleting
			if(verbose) 
			printf("select indicated ready to read, left: %d sec %d usec\n",
					waittime.tv_sec,waittime.tv_usec);
			//decrement front to be appropriate amount of time left
			if(front->next!=back)
			front->next->dtime = waittime.tv_sec*1000 + waittime.tv_usec/1000;

			int recvd = 0;
			//printf("reading...\n");
			recvd = recvfrom(sock,(void*)buffer, sizeof(uint32_t)*3, 0, NULL, NULL);
			if (recvd != sizeof(uint32_t)*3){
				fprintf(stderr,"did not recv amout of bytes expected.\n");
				return -1;
			}
			//buffer = <time,seqnum,respond_port> (network byte order)
			struct Dnode new;
			new.dtime = ntohl(buffer[0]);
			new.seqnum = ntohl(buffer[1]);
			new.portnum = ntohl(buffer[2]);
			if(verbose)
			printf("received %d msec,%d seqnum,%d portnum\n",
					new.dtime, new.seqnum, new.portnum);

			//check for delete
			if(new.dtime < 0){
				if(verbose)
				printf("deleting node with seq num %d\n",new.seqnum);
				deleteNode(new.seqnum);
			}else if(!(new.dtime || new.seqnum || new.portnum)){//all zeros means EXIT
				printf("exiting program\n");
				if(freeNodes()){
					fprintf(stderr,"something went wrong freeing nodes.\n");
					return -1;
				}
				close(outsock);
				close(sock);
				return 0;
			}else{
				//insert
				struct Dnode* newPtr = (struct Dnode*) malloc(sizeof(struct Dnode));
				*newPtr = new;
				if(insert(newPtr)){
					fprintf(stderr,"failed to insert new node\n");
					return -1;
				}
			}
		}
	}
}
