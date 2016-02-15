#include <cstdio>
#include <algorithm>

int main()
{
	int n, m;
	scanf("%d%d", &n, &m);
	
	int remainders[m];
	std::fill(remainders, remainders + m, 0);
	
	remainders[0] = 1;
	
	for (int i = 0, val, sum = 0; i < n; i++) {
		scanf("%d", &val);
		sum = (sum + val) % m;
		remainders[sum]++;
	}
	
	unsigned long long int res = 1;
	
	for (int i = 0; i < m; i++) {
		unsigned long long int remCount = remainders[i];
		res += remCount * (remCount - 1) / 2;
	}
	
	printf("%llu\n", res);
	return 0;
}
