#include <cstdio>

int lower_bound(int array[], int arraySize, int x)
{
	int rangeStart = 0, rangeEnd = arraySize - 1;
	
	if (rangeStart > rangeEnd)
		return -1;
	
	while (rangeStart < rangeEnd) {
		int center = (rangeStart + rangeEnd)/2;
		
		if (x > array[center])
			rangeStart = center + 1;
		else if (x <= array[center])
			rangeEnd = center;
	}
	
	if (array[rangeStart] < x)
		return -1;
	else
		return rangeStart + 1;
}

int main()
{
	int n, x;
	scanf("%d%d", &n, &x);
	
	int array[n];
	for (int i = 0; i < n; i++)
		scanf("%d", &array[i]);
	
	printf("%d\n", lower_bound(array, n, x));
	return 0;
}
