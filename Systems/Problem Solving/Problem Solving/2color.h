#include <stdio.h>
#include <stdlib.h>

typedef struct IntArray {
	int* arr;
	int n;
} IntArray;

//copy the source array into destination array
void copyArray(int dest[], int src[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}

//printing the user defined array - IntArray
void printArray(IntArray* inp) {
	int i;
	for (i = 0; i < inp->n; i++) {
		printf("%d ", inp->arr[i]);
	}
	printf("\n");
}

//comparator function for qsort
int compare(const void* p, const void* q) {
	return *(int*)p - *(int*)q;
}

//method1 for solving two color problem - using qsort function
void two_color_1(int *arr, int n) {
	qsort(arr, n, sizeof(int), compare);
}

//method2 for solving two color problem - counting number of 1's
void two_color_2(int *arr, int n) {
	int i, sum = 0;
	for (i = 0; i < n; i++) {
		sum += arr[i];
	}
	i = 0;
	while(i < sum) {
		arr[i] = 0;
		i++;
	}
	while(i < n) {
		arr[i] = 1;
		i++;
	}
}

//method3 for solving two color problem - swapping numbers using front and back pointers
void two_color_3(int *arr, int n) {
	int beg = 0, end = n - 1;
	do {
		while (arr[beg] != 1 && beg < end) {
			beg++;
		}
		while (arr[end] != 0 && beg < end) {
			end--;
		}
		if (beg < end) {
			arr[beg] = arr[beg] ^ arr[end];
			arr[end] = arr[beg] ^ arr[end];
			arr[beg] = arr[beg] ^ arr[end];
		}
	} while (beg < end);
}

//helper function for providing static array input
IntArray* input() {
	IntArray* int_array = (IntArray*)malloc(sizeof(IntArray));

	//input for 2color
	int n = 12;
	int arr[] = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 };

	int_array->n = n;
	int_array->arr = (int*)malloc(sizeof(int) * n);
	copyArray(int_array->arr, arr, n);

	return int_array;
}

void init() {
	IntArray* integerArray = input();
	printArray(integerArray);
	//two_color_1(integerArray->arr, integerArray->n);
	//two_color_2(integerArray->arr, integerArray->n);
	two_color_3(integerArray->arr, integerArray->n);
	printArray(integerArray);
}