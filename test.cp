int fibo(int num) {
	if(num < 3) return 1;
	return fibo(num - 1) + fibo(num - 2);
}

int main() {
	//callclating 40th fibbonachi number
	print("40th num is %d", fibo(40)); /* i hope this works */
	return -1; 
} 
