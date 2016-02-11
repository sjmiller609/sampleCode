/*Steven Miller
3 semaphores: empty, full, mutex
typedef buffer_item type will be circular queue
that type is stored in header file
buffer can be manipulated with insert_item() and remove_item(), which are called by producer/consumer threads, respectively
*/

#include <stdlib.h> /*required for rand_r*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "buffer.h"

/*max integer to be used in buffer_item*/
//#define RAND_MAX 100
/*the longest a consumer or producer will sleep for in seconds*/
#define MAX_WAIT 5


int insert_item(buffer_item item, buffer_item* buffer, int* next){
	/*insert an item into buffer, used as circular queue*/
	/*next[1] is next to write, after modding by buffer size*/
	buffer[next[1] % BUFFER_SIZE] = item;
	printf("Produced: %d\n", item);

	/*Return 0 if successful*/
	return(0);
}

int remove_item(buffer_item* itemPtr, buffer_item* buffer, int* next){
	/*Remove an object from buffer and place it in item*/
	/*next[0] modded by the buffer size is the next read location (circular queue implementation)*/
	/*don't actually need to get rid of value at that location,
	the empty semaphore will indicate that now that value is garbage
	and it will be overwritten by a producer thread*/
	*itemPtr = buffer[next[0] % BUFFER_SIZE];
	printf("Consumed: %d\n", *itemPtr);

	/*return 0 is successful*/
	return(0);
}

/*initialization function that initializes the mutex along with empty and full (semaphores)*/
int init(sem_t* emptyPtr, sem_t* fullPtr, pthread_mutex_t* mutexPtr){

	int flag = 0;
	if(pthread_mutex_init(mutexPtr, NULL)){
		printf("mutex could not be initialized.\n");
		return -1;
	}
	if(sem_init(emptyPtr, 0, BUFFER_SIZE)){ /*initial size = buffer_size*/
		printf("empty semaphore could not be initialized.\n");
		return -1;
	}
	if(sem_init(fullPtr, 0, 0)){ /*initial size 0*/
		printf("full semaphore could not be initialized");
		return -1;
	}
	/*returns 0 if successful*/
	return(0);
}

/*producer thread*/
void* producer(void *args){

	void** param = (void **) args;
	/*param is void**: an array of generic pointers */
	/*need to cast param[*]  to the correct types*/
	buffer_item* buffer = (buffer_item*) param[0];
	int* next = (int*) param[1];
	int* isEndPtr = (int*) param[2];
	pthread_mutex_t* mutexPtr = (pthread_mutex_t*) param[3];
	sem_t* emptyPtr = (sem_t*) param[4];
	sem_t* fullPtr = (sem_t*) param[5];
	int* seedPtr = (int*) param[6];

	buffer_item rand;
	/*seed is unique from main*/
	unsigned int seed = *seedPtr;

	int go = 1;
	while(go){
		/*generate random number to insert into buffer*/
		rand = rand_r(&seed);
		/*chaining to make seed for next time*/
		seed = rand;
	
		/*sleep for random period of time, less than MAX_WAIT seconds*/
		sleep((int) rand % MAX_WAIT);


		/*pass value, not pointer, don't need any return data except 1 or 0*/
		sem_wait(emptyPtr);
		pthread_mutex_lock(mutexPtr);
		/*critical section*/
		if(*isEndPtr){
			/*this is the exit program section*/
			go = 0;
			sem_post(emptyPtr);
			sem_post(fullPtr);
			pthread_mutex_unlock(mutexPtr);
		}else{
			if(insert_item(rand, buffer, next) < 0){
				printf("producer could not insert item\n");
			}
			/*increment next write index*/
			next[1]++;
			/*end critical section*/
			pthread_mutex_unlock(mutexPtr);
			sem_post(fullPtr);
		}
	}
	return NULL;
}

/*consumer thread*/
void* consumer(void* args){

	void** param = (void **) args;
	/*param is void**: an array of generic pointers */
	/*need to cast param[*]to the correct types*/
	buffer_item* buffer = (buffer_item*) param[0];
	int* next = (int*) param[1];
	int* isEndPtr = (int*) param[2];
	pthread_mutex_t* mutexPtr = (pthread_mutex_t*) param[3];
	sem_t* emptyPtr = (sem_t*) param[4];
	sem_t* fullPtr = (sem_t*) param[5];
	int* seedPtr = (int*) param[6];

	/*the item to be removed, currently empty*/
	buffer_item rand;

	/*seed is unique from main*/
	unsigned int seed = *seedPtr;

	int go = 1;
	while(go){

		/*temporarily use rand to hold random number for sleeping, will overwrite in reading*/
		rand = (buffer_item) rand_r(&seed);
		/*new seed by chaining*/
		seed = rand;
		/*sleep for random period of time, less than MAX_WAIT seconds*/
		sleep((int) rand % MAX_WAIT);
	
		sem_wait(fullPtr);
		pthread_mutex_lock(mutexPtr);
		/*critical section*/
		if(*isEndPtr){
			/*this is the exit program section*/
			go = 0;
			sem_post(fullPtr);
			sem_post(emptyPtr);
			pthread_mutex_unlock(mutexPtr);
		}else{
			if(remove_item(&rand, buffer, next) < 0){
				printf("consumer could not remove item\n");
			}
			/*increment next read index*/
			next[0]++;
			/*end critical section*/
			pthread_mutex_unlock(mutexPtr);
			sem_post(emptyPtr);
		}

		/*rand is going to be lost when overwritten in the next loop,
		 here is where you would type code to use it*/
	}
	return NULL;
}

/*main function, description in comments*/
/*passed 3 parameters in command line*/
int main(int argc, char **argv){

/*Get command line args*/
	/*argv[0] = program name
	1 = how long to sleep (seconds)
	2 = number of producer threads
	3 = number of consumer threads*/

	char* programName = argv[0];
	/*boolean flag for various checks, sometimes used as integer for checks too*/
	int flag = 0;
	/*counting integer for loops*/
	int i = 0;

	/*check for correct number of arguments*/
	if(!(argc==4)){
		printf(" incorrect number of arguments:\n");
		printf(" %s <seconds to sleep> <number of producers> <number of consumers>\n", programName);
		return(-1);
	}

	int sleepSeconds = atoi(argv[1]);
	int producerCount = atoi(argv[2]);
	int consumerCount = atoi(argv[3]);

	/*check to make sure inputs are valid*/
	if(sleepSeconds<=0){
		printf("invaild seconds to sleep, choose greater than 0\n");
		flag = 1;
	}

	if(producerCount<=0){
		printf("invalid number of producers, choose greater than 0\n");
		flag = 1;
	}

	if(consumerCount<=0){
		printf("invalid number of consumers, choose greater than 0\n");
		flag = 1;
	}
	
	if(flag){
		printf(" %s <seconds to sleep> <number of producers> <number of consumers>\n", programName);
		return(-1);
	}

/*initialize buffer, and other variables*/

	/*the buffer */
	/*implemented as a circular array*/
	buffer_item buffer[BUFFER_SIZE];
	int next[2];
	/*next[0] is the next to read location in the circular array*/
	next[0] = 0;
	/*next[1] is the next to write*/
	next[1] = 0;
	int isEnd = 0;

	/*initialize semaphores*/
	pthread_mutex_t mutex;
	sem_t empty;
	sem_t full;

	init(&empty, &full, &mutex);

	/*wikipedia: POSIX threads used for reference with pthread creation*/
	pthread_t consumerThreads[consumerCount];
	pthread_t producerThreads[producerCount];
	/*array of generic pointers, used for parameters in producer/consumer */
	void* thread_args[7];
	thread_args[0] = buffer;
	thread_args[1] = next;
	thread_args[2] = &isEnd;
	thread_args[3] = &mutex;
	thread_args[4] = &empty;
	thread_args[5] = &full;


/*create producer thread(s)*/
	for(i = 0; i < producerCount; i++){
		/*temp is going to be used as seed for rand_r*/
		int temp = i*2;
		thread_args[6] = &temp;
		if(pthread_create(&producerThreads[i], NULL, &producer, thread_args)){
			printf("too many producers: ran out of memory at %d\n", i);
			return -1;
		}
	}

/*create consumer threads*/
	for(i = 0; i < consumerCount; i++){
		/*temp is going to be used as seed for rand_r*/
		int temp = i*2+1;
		thread_args[6] = &temp;
		if(pthread_create(&consumerThreads[i], NULL, &consumer, thread_args)){
			printf("too many consumers: ran out of memory at %d\n", i);
			return -1;
		}
	}

/*sleep for a period of time*/
	sleep(sleepSeconds);
	printf(" time is up\n");

/*release resources*/
	/*signal threads to stop*/
	pthread_mutex_lock(&mutex);
	isEnd = 1;
	pthread_mutex_unlock(&mutex);

	/*wait for threads to complete*/
	printf(" waiting for threads to finish, could take up to %d seconds\n", MAX_WAIT);
	for(i = 0; i < producerCount; i++){
		if(pthread_join(producerThreads[i], NULL)){
			printf("the producers could not be closed successfully\n");
			return -1;
		}
	}
	for(i = 0; i < consumerCount; i++){
		if(pthread_join(consumerThreads[i], NULL)){
			printf("the consumers could not be closed successfully\n");
		}
	}

	/*destroy semaphores and mutex*/
	if(sem_destroy(&full)
	|| sem_destroy(&empty)
	|| pthread_mutex_destroy(&mutex)){
		printf("garbage collection failed.\n");
	}
/*terminate application*/
	printf(" done.\n");
	return(0);
}
