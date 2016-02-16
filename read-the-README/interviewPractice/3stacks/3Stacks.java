class Stacks3 {

	T[] array;
	int size;
	int size1;
	int size2;
	int size3;

	public Stacks3(T[] inputarray, int totalsize){
		this.array = inputarray;
		this.size = totalsize;
	}
	
	public void push1(int stack, T x){
		if(!(stack>0 && stack<4)){
			return;
		}
	}

	public T pop1(int stack, T x){
		if(!(stack>0 && stack<4)){
			return null;
		}
	}

}
