#include <cstdio>
#include <vector>
#include <set>

using namespace std;

struct Node;

struct AdjNode {
	Node* node;
	int distance;
};

struct Node {
	int number, pathLength;
	vector<AdjNode> adjNodes;
};

AdjNode make_adjNode(Node* adjNode, int dist)
{
	AdjNode res;
	res.node = adjNode;
	res.distance = dist;
	return res;
}

Node* make_node(int n)
{
	Node* node = new Node();
	node->number = n;
	node->pathLength = -1;
	return node;
}

struct cmp {
	bool operator()(const Node* a, const Node* b) {
		if (a->pathLength == -1)
			return false;	// b < a || a == b == -1
		else if (b->pathLength == -1)
			return true;	// a < b
		else if (a->pathLength != b->pathLength)
			return a->pathLength < b->pathLength;
		return a->number < b->number;
	}
};

int main()
{
	int nodesCount, edgesCount;
	scanf("%d%d", &nodesCount, &edgesCount);
	
	if (nodesCount == 0)
		return 0;
	
	Node* nodes[nodesCount];
	set<Node*, cmp> nodesSet;
	
	for (int i = 0; i < nodesCount; i++)
		nodes[i] = make_node(i);
	
	for (int i = 0, a, b, dist; i < edgesCount; i++) {
		scanf("%d%d%d", &a, &b, &dist);
		a--; b--;
		
		nodes[a]->adjNodes.push_back(make_adjNode(nodes[b], dist));
		nodes[b]->adjNodes.push_back(make_adjNode(nodes[a], dist));
	}
	
	nodes[0]->pathLength = 0;
	
	for (int i = 0; i < nodesCount; i++)
		nodesSet.insert(nodes[i]);
	
	while (!nodesSet.empty()) {
		Node* currentNode = *nodesSet.begin();
		nodesSet.erase(nodesSet.begin());
		
		if (currentNode->pathLength == -1)
			continue;
		
		for (unsigned int i = 0; i < currentNode->adjNodes.size(); i++) {
			Node* nextNode = currentNode->adjNodes[i].node;
			int distance = currentNode->adjNodes[i].distance;
			
			if (nextNode->pathLength == -1 || nextNode->pathLength > currentNode->pathLength + distance) {
				nodesSet.erase(nextNode);
				nextNode->pathLength = currentNode->pathLength + distance;
				nodesSet.insert(nextNode);
			}
		}
	}
	
	for (int i = 0; i < nodesCount; i++) {
		printf("%d ", nodes[i]->pathLength);
		delete nodes[i];
	}
	
	printf("\n");
	return 0;
}
