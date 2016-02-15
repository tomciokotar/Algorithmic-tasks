#include <cstdio>
#include <vector>

const int maxn = 500000;

int n, m;
long long int traffic[maxn], maxTraffic = 0;
int pre[maxn], post[maxn], order = 0, ancestors[maxn][19];
std::vector<int> adjList[maxn];

void setOrders(int node)
{
	pre[node] = order++;
	
	for (unsigned int i = 0; i < adjList[node].size(); i++)
		if (adjList[node][i] != ancestors[node][0]) {
			ancestors[ adjList[node][i] ][0] = node;
			setOrders(adjList[node][i]);
		}
	
	post[node] = order++;
}

bool isAncestor(int node, int anc)
{
	return pre[anc] < pre[node] && post[node] < post[anc];
}

int getLca(int a, int b)
{
	if (isAncestor(a, b))
		return b;
	else if (isAncestor(b, a))
		return a;
		
	int node = a, lvl = 18;
	while (!isAncestor(b, ancestors[node][0])) {
		while (isAncestor(b, ancestors[node][lvl]))
			lvl--;
		
		node = ancestors[node][lvl];
	}
	
	return ancestors[node][0];
}

void setMaxTraffic(int node)
{
	for (unsigned int i = 0; i < adjList[node].size(); i++)
		if (adjList[node][i] != ancestors[node][0]) {
			setMaxTraffic(adjList[node][i]);
			traffic[node] += traffic[ adjList[node][i] ];
		}
	
	if (traffic[node] > maxTraffic)
		maxTraffic = traffic[node];
}

int main()
{
	scanf("%d%d", &n, &m);
	
	for (int i = 0, a, b; i < n-1; i++) {
		scanf("%d%d", &a, &b);
		
		adjList[a].push_back(b);
		adjList[b].push_back(a);
	}
	
	ancestors[0][0] = 0;
	setOrders(0);
	
	for (int lvl = 1; lvl < 19; lvl++)
		for (int i = 0; i < n; i++)
			ancestors[i][lvl] = ancestors[ ancestors[i][lvl-1] ][lvl - 1];
	
	int a, b, k;
	while (m--) {
		scanf("%d%d%d", &a, &b, &k);
		
		if (isAncestor(a, b)) {
			traffic[a] += k;
			traffic[b] -= k;
		}
		else if (isAncestor(b, a)) {
			traffic[b] += k;
			traffic[a] -= k;
		}
		else {
			traffic[a] += k;
			traffic[b] += k;
			traffic[getLca(a, b)] -= 2*k;
		}
	}
	
	setMaxTraffic(0);
	
	printf("%lld\n", maxTraffic);
	return 0;
}
