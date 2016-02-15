#include <cstdio>
#include <set>

using namespace std;

const int inf = 1000000001;

int n, k, nodesCount;
int* dist;

struct cmp
{
	bool operator()(const int &a, const int &b)
	{
		if (dist[a] != dist[b])
			return dist[a] < dist[b];
		return a < b;
	}
};

set<int, cmp> nodesSet;

int main()
{
	scanf("%d", &n);
	
	int edges[n];
	for (int i = 0; i < n; i++)
		scanf("%d", &edges[i]);
	
	nodesCount = edges[0];
	dist = new int[nodesCount];
	
	dist[0] = 0;
	for (int i = 1; i < nodesCount; i++)
		dist[i] = inf;
	
	for (int i = 0; i < nodesCount; i++)
		nodesSet.insert(i);
	
	while (!nodesSet.empty()) {
		int node = *nodesSet.begin();
		nodesSet.erase(nodesSet.begin());
		
		if (dist[node] >= inf)
			continue;
		
		for (int i = 1; i < n; i++) {
			int nextNode = (node + edges[i]) % nodesCount;
			
			if (dist[nextNode] > dist[node] + edges[i]) {
				nodesSet.erase(nodesSet.find(nextNode));
				dist[nextNode] = dist[node] + edges[i];
				nodesSet.insert(nextNode);
			}
		}
	}
	
	scanf("%d", &k);
	
	while (k--) {
		int a;
		scanf("%d", &a);
		
		if (dist[a % nodesCount] <= a)
			printf("TAK\n");
		else
			printf("NIE\n");
	}
	
	delete[] dist;
	
	return 0;
}
