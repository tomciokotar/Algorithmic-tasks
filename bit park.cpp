#include <cstdio>
#include <algorithm>

struct Node
{
	int parent, leftSon, rightSon;
};

struct Ancestor
{
	int node, dist, whereToGo;
};

const int maxn = 500000;

int n, m;
Node nodes[maxn+1];
int leftDepth[maxn+1], rightDepth[maxn+1];
int nodeUp[maxn+1][19], nodeDown[maxn+1][19];
Ancestor bestAncestors[maxn+1];

void setDepth(int i)
{
	if (i == -1)
		return;
	
	int leftSon = nodes[i].leftSon, rightSon = nodes[i].rightSon;
	
	setDepth(leftSon);
	setDepth(rightSon);
	
	if (leftSon == -1)
		leftDepth[i] = 0;
	else
		leftDepth[i] = std::max(leftDepth[leftSon], rightDepth[leftSon]) + 1;
	
	if (rightSon == -1)
		rightDepth[i] = 0;
	else
		rightDepth[i] = std::max(leftDepth[rightSon], rightDepth[rightSon]) + 1;
}

int getMaxDist(Ancestor &anc)
{
	if (anc.whereToGo == 1)
		return anc.dist + leftDepth[anc.node];
	else
		return anc.dist + rightDepth[anc.node];
}

int getMaxDist(int i)
{
	return getMaxDist(bestAncestors[i]);
}

Ancestor makeAncestor(int node, int dist, int whereToGo)
{
	Ancestor anc;
	anc.node = node;
	anc.dist = dist;
	anc.whereToGo = whereToGo;
	return anc;
}

void setBestAncestor(int i, Ancestor bestAnc)
{
	int left = nodes[i].leftSon, right = nodes[i].rightSon;
	
	if (getMaxDist(bestAnc) >= getMaxDist(i))
		bestAncestors[i] = bestAnc;
	
	if (left != -1) {
		if (getMaxDist(bestAnc) < rightDepth[i])
			setBestAncestor(left, makeAncestor(i, 1, 2));
		else
			setBestAncestor(left, makeAncestor(bestAnc.node, bestAnc.dist + 1, bestAnc.whereToGo));
	}
	
	if (right != -1) {
		if (getMaxDist(bestAnc) < leftDepth[i])
			setBestAncestor(right, makeAncestor(i, 1, 1));
		else
			setBestAncestor(right, makeAncestor(bestAnc.node, bestAnc.dist + 1, bestAnc.whereToGo));
	}
}

int getPowerFloor(int distGiven)
{
	int pow = 0;
	int currentDist = 1;
	
	while (2 * currentDist <= distGiven) {
		currentDist *= 2;
		pow++;
	}
	
	return pow;
}

int getNodeUp(int i, int dist)
{
	if (i == -1)
		return -1;
	
	int pow = getPowerFloor(dist);
	return (dist == 0) ? i : getNodeUp(nodeUp[i][pow], dist - (1 << pow));
}

int getNodeDown(int i, int dist)
{
	if (i == -1)
		return -1;
	
	int pow = getPowerFloor(dist);
	return (dist == 0) ? i : getNodeDown(nodeDown[i][pow], dist - (1 << pow));
}

int main()
{
	scanf("%d", &n);
	
	for (int i = 1, a, b; i <= n; i++) {
		scanf("%d%d", &a, &b);
		
		nodes[i].leftSon = a;
		nodes[i].rightSon = b;
		
		if (a != -1)
			nodes[a].parent = i;
		if (b != -1)
			nodes[b].parent = i;
	}
	
	nodes[1].parent = -1;
	setDepth(1);
	
	for (int i = 1; i <= n; i++) {
		nodeUp[i][0] = nodes[i].parent;
		
		if (leftDepth[i] >= rightDepth[i])
			nodeDown[i][0] = nodes[i].leftSon;
		else
			nodeDown[i][0] = nodes[i].rightSon;
	}
	
	for (int lvl = 1; lvl < 19; lvl++)
		for (int i = 1; i <= n; i++) {
			nodeUp[i][lvl] = (nodeUp[i][lvl-1] == -1)
							? -1 : nodeUp[ nodeUp[i][lvl-1] ][lvl - 1];
			nodeDown[i][lvl] = (nodeDown[i][lvl-1] == -1)
							? -1 : nodeDown[ nodeDown[i][lvl-1] ][lvl - 1];
		}
	
	for (int i = 1; i <= n; i++) {
		bestAncestors[i].node = i;
		bestAncestors[i].dist = 0;
		
		if (leftDepth[i] >= rightDepth[i])
			bestAncestors[i].whereToGo = 1;
		else
			bestAncestors[i].whereToGo = 2;
	}
	
	if (nodes[1].leftSon != -1)
		setBestAncestor(nodes[1].leftSon, makeAncestor(1, 1, 2));
	if (nodes[1].rightSon != -1)
		setBestAncestor(nodes[1].rightSon, makeAncestor(1, 1, 1));
	
	scanf("%d", &m);
	
	int a, d;
	while (m--) {
		scanf("%d%d", &a, &d);
		
		Ancestor &bestAnc = bestAncestors[a];
		
		if (bestAnc.dist == d)
			printf("%d\n", bestAnc.node);
		else if (bestAnc.dist > d)
			printf("%d\n", getNodeUp(a, d));
		else {
			if (bestAnc.whereToGo == 1)
				printf("%d\n", getNodeDown(nodes[bestAnc.node].leftSon, d - bestAnc.dist - 1));
			else
				printf("%d\n", getNodeDown(nodes[bestAnc.node].rightSon, d - bestAnc.dist - 1));
		}
	}
	
	return 0;
}
