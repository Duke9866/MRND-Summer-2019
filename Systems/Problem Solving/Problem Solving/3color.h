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

//method1 for solving three color problem - using qsort function
void three_color_1(int *arr, int n) {
	qsort(arr, n, sizeof(int), compare);
}

//method2 for solving three color problem - using 3-way partitioning
void three_color_2(int *arr, int n) {
	int low = 0;
	int high = n - 1;
	int mid = 0;
	while (mid <= high)
	{
		switch (arr[mid])
		{
		case 0:
			arr[low] = arr[low] ^ arr[mid];
			arr[mid] = arr[low] ^ arr[mid];
			arr[low] = arr[low] ^ arr[mid];
			low++;
			mid++;
			break;
		case 1:
			mid++;
			break;
		case 2:
			arr[mid] = arr[mid] ^ arr[high];
			arr[high] = arr[mid] ^ arr[high];
			arr[mid] = arr[mid] ^ arr[high];
			high--;
			break;
		}
	}
}

//helper function for providing static array input
IntArray* input() {
	IntArray* int_array = (IntArray*)malloc(sizeof(IntArray));

	//input for 3color
	int n = 18;
	int arr[] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0 };

	int_array->n = n;
	int_array->arr = (int*)malloc(sizeof(int) * n);
	copyArray(int_array->arr, arr, n);

	return int_array;
}

void init() {
	IntArray* integerArray = input();
	printArray(integerArray);
	//three_color_1(integerArray->arr, integerArray->n);
	three_color_2(integerArray->arr, integerArray->n);
	printArray(integerArray);
}