#include <stdio.h>
#include <stdlib.h>

//recursive sum function
long long rsum(int *arr, int n) {
	if (n < 0) {
		return -1;
	}
	else if (n == 1) {
		return arr[n - 1];
	}
	else {
		return arr[n - 1] + rsum(arr, n - 1);
	}
}

long fib_memoization(long *mem, long n) {
	if (n < 1) {
		return -1;
	}
	if (mem[n] != 0) {
		return mem[n];
	}
	else if (n == 1 || n == 2) {
		mem[n] = 1;
		return mem[n];
	}
	else {
		mem[n] = fib_memoization(mem, n - 1) + fib_memoization(mem, n - 2);
		return mem[n];
	}
}

void printNumber(int* digits, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%d", digits[i]);
	}
	printf("\n");
}

void swap(int *a, int *b) {
	*a = *a ^ * b;
	*b = *a ^ * b;
	*a = *a ^ * b;
}

void init() {
	//int arr[] = { 2147483647, 2147483647, 2147483647 };
	int n = 6;
	long* fib = (long*)calloc(n > 0 ? n + 1 : 1, sizeof(long));
	long fib_n = fib_memoization(fib, n);
	printf("%d\n", fib_n);
}