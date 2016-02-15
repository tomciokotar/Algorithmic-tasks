#include <cstdio>

const int modVal = 1000000000;

struct Result
{
	int lastFirst;
	int lastLast;
};

int countSequences(int array[], int size)
{
	Result res[size][size];
	
	if (size == 1)
		return 1;
	
	for (int i = 0; i < size - 1; i++) {
		if (array[i] < array[i+1])
			res[i][i+1].lastFirst = res[i][i+1].lastLast = 1;
		else
			res[i][i+1].lastFirst = res[i][i+1].lastLast = 0;
	}

	for (int i = 3; i <= size; i++)
		for (int begin = 0, end = i-1; end < size; begin++, end++) {
			res[begin][end].lastFirst = res[begin][end].lastLast = 0;
			
		if (array[end] > array[begin]) {
			res[begin][end].lastLast += res[begin][end - 1].lastFirst;
			res[begin][end].lastLast %= modVal;
		}
		if (array[end] > array[end - 1]) {
			res[begin][end].lastLast += res[begin][end - 1].lastLast;
			res[begin][end].lastLast %= modVal;
		}
		if (array[begin] < array[begin + 1]) {
			res[begin][end].lastFirst += res[begin + 1][end].lastFirst;
			res[begin][end].lastFirst %= modVal;
		}
		if (array[begin] < array[end]) {
			res[begin][end].lastFirst += res[begin + 1][end].lastLast;
			res[begin][end].lastFirst %= modVal;
		}
	}

	return (res[0][size - 1].lastFirst + res[0][size - 1].lastLast) % modVal;
}

int main()
{
	int n;
	scanf("%d", &n);
	
	int arr[n];
	
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	
	printf("%d\n", countSequences(arr, n));
	return 0;
}

