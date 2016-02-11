/*Steven Miller
miller.5252@buckeyemail.osu.edu
no partner

compile with:
gcc -O1 -Wall -o lab4 lab4.c -lpthread
run with:
./lab4

Lab 4
solutions for matrix multiplication
A: n x m, B: m x p
find AB
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

/*matrix dimensions*/
#define N_VAL 2400
#define M_VAL 2000
#define P_VAL 500

/*initialize variables with file scope*/
static int A[N_VAL][M_VAL];
static int B[M_VAL][P_VAL];

/*initialize output arrays, file scope*/
static int C1[N_VAL][P_VAL];
static int C[N_VAL][P_VAL];
/*number of threads, file scope*/
int numThreads = 0;

/*this method puts ints into the two arrays*/
/*it's not really a part of the performace criteria*/
int initializeMatricies(){

	/*two indicies*/
	int i = 0;
	int j = 0;

	/*initialize matricies*/

	printf("initializing A");
	/*matrix A*/
	printf("\n");
	for(i = 0; i < N_VAL; i++){
		printf("\r%d %%",i*100/N_VAL);

		for(j = 0; j < M_VAL; j++){
			/*as defined in project desciption*/
			A[i][j] = j - i + 2;
		}
	}
	printf("\r%d %%\n",100);
	printf("A is initialized!\n");

	/*matrix B*/
	for(i = 0; i < M_VAL; i++){
		printf("\r%d %%",i*100/M_VAL);
		for(j = 0; j < P_VAL; j++){
			/*as defined in project description*/
			B[i][j] = i - j + 1;
		}
	}
	printf("\r%d %%\n",100);

	/*return 0 if completed successfully*/
	printf("B is initialized\n");
	return 0;
}

/*calculates and inserts one element of array*/
void calcElement(int row, int column){

	int i;
	/*initialize to zero*/
	C[row][column] = 0;
	/*dot product of row of A and column of B*/
	for(i = 0; i < M_VAL; i++){
		/*the product of an element of A and an element of B*/
		C[row][column] += (A[row][i]) * (B[i][column]);
	}
}

/*this method is just for the sinlge thread version*/
int multiply(){
/*
A: n x m, B: m x p
find AB: n x p
*/
	int i;
	int j;
	int k;

	/*this loop sequence is extremely parallel*/
	for(i = 0; i < N_VAL; i++){
		for(j = 0; j < P_VAL; j++){

			/*calculate and put the value at that address*/
			/*initialize to zero*/
			int result = 0;

			/*dot product of row of A and column of B*/
			for(k = 0; k < M_VAL; k++){
				/*the product of an element of A and an element of B*/
				result += (A[i][k]) * (B[k][j]);
			}
			
			C1[i][j] = result;
			
		}
	}

	return 0;
}

/*returns 1 when there aren't any more valid for next*/
/*this logic only works assuming that number of threads is less than number of columns*/
int incrementNext(int next[], int num){

	// go down row (move right in matrix),
	// skipping where other threads have claimed
	next[1]++;

	if(next[1] >= P_VAL){

		next[1] = 0;
		next[0] += numThreads;
		if(next[0] >= N_VAL){
			/*we don't have any rows left to calculate*/
			return 1;
		}
	}

	return 0;
}

void* perform_work(void* args){

	int num = *((int *) args);
	int next[2] = {num,0};

	//int next[2] = {num,0};
	/*not going to stop until all elements are finished*/
	while(1){

		/*in this loop is where threads will spend the majority of their time*/
		/*calulate the element in C at row = next[0], column = next[1]*/
		calcElement(next[0],next[1]);
		if(incrementNext(next,num)){
			return 0;
		}
	}
}

int multiplyMulti(){

/*
A: n x m, B: m x p
find AB: n x p
*/

	int i;
	pthread_t threads[numThreads];
	//this is so each thread will have a place it can touch its respective index
	int forthreads[numThreads];
	/*make all threads*/
	for(i = 0; i < numThreads; i++){
		forthreads[i] = i;
		if(pthread_create(&threads[i], NULL, &perform_work,(void*)&(forthreads[i]))){
			printf("unable to create thread number %d\n", i);
		}
	}

	/*waiting for threads to be done*/
	for(i = 0; i < numThreads; i++){
		if(pthread_join(threads[i], NULL)){
			printf("unable to close thread %d\n", i);
		}
	}

	return 0;
}

/*compares two arrays*/
int compare(){

	int i;
	int j;
	
	for(i = 0; i < N_VAL; i++){
		for(j = 0; j < P_VAL; j++){
			/*check if they are equal*/
			if(C[i][j] != C1[i][j]){
				fprintf(stderr,"%d != %d\ni = %d, j = %d\n",C[i][j],C1[i][j],i,j);
				return -1;
			}
		}
	}
	return 0;

}

int main(int argc, char** argv){

	if(argc!=2){
		printf("incorrect number of arguments.\n");
		printf("lab4 <number of threads max>\n");
		return -1;
	}

	char* totalString = argv[1];
	int total = atoi(totalString);
	/*A,B are already pointers*/
	initializeMatricies();
	printf("going to calculate with up to %d threads\n",total);

	printf("now calculating with 1 thread...\n");
	/*start timer*/
	struct timeval start, end;
	int elapsed;

	gettimeofday(&start,NULL);
	if(multiply()){
		printf("something went wrong in multiply()\n");
	}
	/*stop timer*/
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
	printf("completed multiplication using 1 threads in %d milliseconds\n",elapsed);


	for(numThreads = 2; numThreads < total+1 ; numThreads++){
		
		printf("now calculating with %d threads...\n", numThreads);
		/*start timer*/
		gettimeofday(&start, NULL);
		if(multiplyMulti()){
			printf("something went wrong in multiply()\n");
		}

		/*stop timer*/
		gettimeofday(&end, NULL);
		elapsed = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
		printf("completed multiplication using %d threads in %d milliseconds\n",numThreads, elapsed);
		/*compare matrix result*/
		if(compare()){
			printf("the matrix produced does not match\n");
		}else{
			printf("the matrix produced matches!\n");
		}

	}
	printf("done");
	return 0;
}
