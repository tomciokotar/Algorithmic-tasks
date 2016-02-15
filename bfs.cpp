#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

const int maxNodes = 100000;

int nodesCount, edgesCount, pathLength[maxNodes];
bool visited[maxNodes];
queue<int> nodesQueue;
vector<int> adjNodes[maxNodes];

int main()
{
	scanf("%d%d", &nodesCount, &edgesCount);
	
	for (int i = 0, a, b; i < edgesCount; i++) {
		scanf("%d%d", &a, &b);
		a--; b--;
		
		adjNodes[a].push_back(b);
		adjNodes[b].push_back(a);
	}
	
	pathLength[0] = 0;
	visited[0] = true;
	nodesQueue.push(0);
	
	while (!nodesQueue.empty()) {
		int curr = nodesQueue.front();
		nodesQueue.pop();
		
		for (unsigned int i = 0; i < adjNodes[curr].size(); i++) {
			int next = adjNodes[curr][i];
			
			if (!visited[next]) {
				visited[next] = true;
				pathLength[next] = pathLength[curr] + 1;
				nodesQueue.push(next);
			}
		}
	}
	
	for (int i = 0; i < nodesCount; i++)
		printf("%d ", pathLength[i]);
	
	printf("\n");
	return 0;
}
