#include <cstdio>

int nwd(int a, int b)
{
	while (b != 0) {
		int z = b;
		b = a%b;
		a = z;
	}
	
	return a;
}

int main()
{
	int n;
	scanf("%d", &n);
	
	unsigned long long int a, b;
	while (n--) {
		scanf("%llu%llu", &a, &b);
		
		if (nwd(a, b) != 1 || a == 1 || b == 1)
			printf("-1\n");
		else
			printf("%llu\n", a*b - a - b);
	}
	
	return 0;
}
