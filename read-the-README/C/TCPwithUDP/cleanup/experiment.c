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
int statictest(){
	static int test = 100;
	test++;
	return test;
}
int main()
{
	int i = 0;
	for(;i<10;i++){
	printf("%d\n",statictest());
	}
	return 0;
}
