//absolute value
int abs(int x){
	if(x<0){
		return -1*x;
	}else{
		return x;
	}
}

//reduces an address to the finite address space
int spaceTransform(int** array,int* inputarray,int initialsize){
	if((*array-inputarray) >= initialsize){
		*array = inputarray + (inputarray-*array)%initialsize;
	}
}

//assming unique elements in array
//inputarray points to the first element of the input array
//array points to the first element of the sub array this call is on
//size is the size of the current array we are looking at
//initialsize is the size of the input array
int* getLowest(int* inputarray,int* array, int size,int intitialsize){

	//transform indicies to finite number space
	spaceTransform(&array,inputarray,initialsize);
	//our current check index
	int index = 0;
	if(size >= 3){

		//pick 3 integers, evenly spaced from cirular array
		int* x = array + 0;
		int* y = array + size/3;
		int* z = array + 2*size/3;
		int distzx = z - x;
		int distxy = y - x;
		int distyz = z - y;

		//transform indicies to finite number space
		spaceTransform(&y,inputarray,initialsize);
		spaceTransform(&z,inputarray,initialsize);

		//see diagram
		//x<y<z
		if(*x<*y){
			if(*y<*z){
				return getLowest(inputarray, z+1,distzx,initialsize);
			}else{
				return getLowest(inputarray, y+1,distyz,initialsize);
			}
		}else
		//y<z<x
		if(*y<*z){
			if(*z<*x){
				return getLowest(inputarray, x+1,distxy,initialsize);
			}else{
				return getLowest(inputarray, y, distyz,initialsize);
			}
		}else
		//now x>y>z -> x>z
		//z<y<x
		if(){
			
			return getLowest(inputarray, z,distzx,initialsize);
		}else
		//x<z<y
		if(*x<*z && *z<*y){
			return getLowest(inputarray, x,distxy,initialsize);
		}else
		else{
			//nothing here
			return NULL;
		}

	}else if(size >0){
		int* smallest = array;
		for(int i = 1;i<size;i++){
			if(*(array+i) < *smallest){
				smallest = array+i;
				spaceTransform(&smallest,inputarray,initialsize);
			}
		}
		return smallest;
	}
}
