#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

/* this function is run by the second thread */
void *inc_x(void *x_void_ptr)
{
	/* increment x to 100 */
	sleep(1);
	int *x_ptr = (int *)x_void_ptr;
	while(++(*x_ptr) < 100);
	printf("x increment finished\n");
	/* the function must return something - NULL will do */
	return NULL;
}

int createThread(int* xptr){
	/* this variable is our reference to the second thread */
	pthread_t inc_x_thread;
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, inc_x, xptr)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	return 0;
}
int main()
{
	/* wait for the second thread to finish */
	int x;
	int y;
	createThread(&x);
	/* increment y to 100 in the first thread */
	while(++y < 100);
	printf("y increment finished\n");
/*
	if(pthread_join(inc_x_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}
*/
	/* show the results - x is now 100 thanks to the second thread */
	sleep(2);
	printf("x: %d, y: %d\n", x, y);
	return 0;
}
