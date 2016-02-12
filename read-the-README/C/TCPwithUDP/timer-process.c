//Delta-list implementaion for timer this process receives on a port a packet indicating amount of time and a port to respond to
//incoming packet UDP data: time (32 bits), seqnum (32 bits), port num to respond to (32 bits)
//send packet data: seqnum (32 bits)

struct Dnode{
	int dtime;
	int portnum;
	int seqnum;
	struct Dnode* next;
	struct Dnode* prev;
}
//pointers to the two dummy nodes at front / back of doubly linked list
struct Dnode* front;
struct Dnode* back;

int insert(struct Dnode* insertme){
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
//returns -1 if invalid port number
//otherwise returns the port number
int portNum(char port[])
{
	int i = 0;
	//checking for negative numbers
	if (port[0] == '-') return false;
	for (; port[i] != '\0'; i++){
		if (!isdigit(port[i])) return -1;
	}
	i = atoi(port);
	if(i < 1024 || i > 65525) return -1;
	return atoi(port);
}

int main(int argc, char* argv[]){
	int port;
	//get port from input, make sure it's valid and suck
	if(arc < 2 || (port = portNum(argv[1])<0 )){
		fprintf(stderr,
		"timer-process failed to start: invalid port number");
		return -1;
	}
	//intitialize doubly linked list to implement delta-timer
	//using two dummy nodes
	struct Dnode dummyfront;
	struct Dnode dummyend;
	dummyfront.next = &dummyback;
	dummyfront.prev = NULL;
	dummyback.prev = &dummyfront
	dummyback.next = NULL;
	front = &dummyfront;
	back = &dummyback;
}

