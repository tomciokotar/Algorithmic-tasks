#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

struct Node {
	int number;
	vector<int> adjNodes;
};

int nodesCount, edgesCount;
queue<int> nodesQueue;

int main()
{
	scanf("%d%d", &nodesCount, &edgesCount);
	
	if (nodesCount == 0)
		return 0;
	
	bool visited[nodesCount];
	int pathLength[nodesCount];
	Node nodes[nodesCount];
	
	for (int i = 0; i < nodesCount; i++) {
		visited[i] = false;
		pathLength[i] = -1;
		nodes[i].number = i;
	}
	
	for (int i = 0, a, b; i < edgesCount; i++) {
		scanf("%d%d", &a, &b);
		a--; b--;
		
		nodes[a].adjNodes.push_back(b);
		nodes[b].adjNodes.push_back(a);
	}
	
	visited[0] = true;
	pathLength[0] = 0;
	nodesQueue.push(0);
	
	while (!nodesQueue.empty()) {
		Node currentNode = nodes[ nodesQueue.front() ];
		nodesQueue.pop();
		
		for (unsigned int i = 0; i < currentNode.adjNodes.size(); i++) {
			Node nextNode = nodes[ currentNode.adjNodes[i] ];
			
			if (!visited[nextNode.number]) {
				visited[nextNode.number] = true;
				pathLength[nextNode.number] = pathLength[currentNode.number] + 1;
				nodesQueue.push(nextNode.number);
			}
		}
	}
	
	for (int i = 0; i < nodesCount; i++)
		printf("%d ", pathLength[i]);
	
	printf("\n");
	return 0;
}
