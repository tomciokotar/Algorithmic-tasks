#include <cstdio>
#include <list>

using namespace std;

struct Temp
{
	int highest;
	int lowest;
};

Temp make_temp(int lowest, int highest)
{
	Temp res;
	res.lowest = lowest;
	res.highest = highest;
	return res;
}

const int maxn = 1000000;

int n, result = 1, rangeStart = 0;
Temp temps[maxn];
list<int> tempsList;

int main()
{
	scanf("%d", &n);
	
	for (int i = 0, a, b; i < n; i++) {
		scanf("%d%d", &a, &b);
		temps[i] = make_temp(a,b);
	}
	
	tempsList.push_back(0);
	
	for (int i = 1; i < n; i++) {
		while (!tempsList.empty() && temps[ tempsList.front() ].lowest > temps[i].highest) {
			rangeStart = tempsList.front() + 1;
			tempsList.pop_front();
		}
		
		if (i - rangeStart + 1 > result)
			result = i - rangeStart + 1;
		
		while (!tempsList.empty() && temps[i].lowest >= temps[ tempsList.back() ].lowest)
			tempsList.pop_back();
		
		tempsList.push_back(i);
	}
	
	printf("%d\n", result);
	return 0;
}
