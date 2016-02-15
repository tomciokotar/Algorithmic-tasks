#include <cstdio>
#include <vector>
#include <queue>
#include <list>
#include <stack>
#include <set>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// 1. Binary search in an array that's ascending to some point and then descending - O(log n)
int findBiggest(int array[], int size)
{
	int start = 0, end = size - 1;
	
	while (start < end) {
		int mid = (start + end)/2;
		
		if (array[mid] < array[mid + 1])
			start = mid + 1;
		else
			end = mid;
	}
	
	return array[start];
}

// 2. Binary search in an ascending array after some circular shifts - O(log n)
int findHighest(int array[], int size)
{
    if (size == 1)
        return array[0];
    
	int start = 0, end = size - 2;

	if (array[0] < array[size - 1])
		return array[size - 1];

	while (end - start > 1) {
		int mid = (start + end)/2;
		
		if (array[mid] < array[size - 1])
			end = mid - 1;
		else
			start = mid;
	}

	if (start == end - 1)
		return max(array[start], array[end]);
	else
		return array[start];
}

// 3. Takes two ascending arrays of size n and tells what'll be the n-th element after merging them - O(log^2 n)
int findLast(int array[], int size, int x)
{
	int start = 0, end = size - 1;
	
	while (end - start > 1) {
		int mid = (start + end)/2;

		if (array[mid] > x)
			end = mid - 1;
		else
			start = mid;
	}

	if (array[end] <= x)
		return end;
	else if (array[start] <= x)
		return start;
	else
		return -1;
}

int findNth(int arrayA[], int arrayB[], int n)
{
	int start = 0, end = n - 1;

	while (start <= end) {
		int mid = (start + end)/2;
		int pos = findLast(arrayB, n, arrayA[mid] - 1) + mid + 1;	

		if (pos == n)
			return arrayA[mid];
		else if (pos > n)
			end = mid - 1;
		else
			start = mid + 1;
	}

	start = 0, end = n - 1;
	
	while (start <= end) {
		int mid = (start + end)/2;
		int pos = findLast(arrayA, n, arrayB[mid]) + mid + 1;

		if (pos == n)
			return arrayB[mid];
		else if (pos > n)
			end = mid - 1;
		else
			start = mid + 1;
	}
	
	return -1;
}

// 4. Returns the number of cycles in a cactus graph - O(n)
void dfs(int node, int last, vector<int> adjList[], bool visited[], int &result)
{
	visited[node] = true;

	for (unsigned int i = 0; i < adjList[node].size(); i++) {
		if (!visited[ adjList[node][i] ])
			dfs(adjList[node][i], node, adjList, visited, result);
		else if (adjList[node][i] != last)
			result++;
	}
}

int findCycles(vector<int> adjList[], int nodesCount)
{
	bool visited[nodesCount];
	int result = 0;
	
	fill(visited, visited + nodesCount, false);
	
	for (int i = 0; i < nodesCount; i++)
		if (!visited[i])
			dfs(i, -1, adjList, visited, result);
	
	return result/2;
}

// 5. How long will it take for the dog to catch the rabbit - O(n*(n+m))
int getMaxDistance(vector<int> adjList[], int nodesCount, int startNode, int maxSteps)
{
	bool visited[nodesCount];
	int dist[nodesCount], currMax = 0;
	queue<int> nodesQueue;
	
	fill(visited, visited + nodesCount, false);
	
	dist[startNode] = 0;
	visited[startNode] = true;
	nodesQueue.push(startNode);
	
	while (!nodesQueue.empty()) {
		int node = nodesQueue.front();
		nodesQueue.pop();
		currMax = max(currMax, dist[node]);
		
		for (unsigned int i = 0; i < adjList[node].size(); i++) {
			int nextNode = adjList[node][i];
			
			if (!visited[nextNode]) {
				if (dist[node] == maxSteps)
					return -1;
				
				visited[nextNode] = true;
				dist[nextNode] = dist[node] + 1;
				nodesQueue.push(nextNode);
			}
		}
	}
	
	return currMax;
}

int catchTheRabbit(vector<int> adjList[], int nodesCount, int dogNode, int rabbitNode, int maxSteps)
{
	bool visited[nodesCount];
	int dist[nodesCount], maxDistance[nodesCount];
	queue<int> nodesQueue;
	
	fill(visited, visited + nodesCount, false);
	
	dist[dogNode] = 0;
	visited[dogNode] = true;
	nodesQueue.push(dogNode);

	while (!nodesQueue.empty()) {
		int node = nodesQueue.front();
		nodesQueue.pop();

		for (unsigned int i = 0; i < adjList[node].size(); i++) {
			int nextNode = adjList[node][i];
			
			if (!visited[nextNode]) {
				if (nextNode == rabbitNode && dist[node] < maxSteps)
					return dist[node] + 1;
	
				visited[nextNode] = true;
				dist[nextNode] = dist[node] + 1;
				nodesQueue.push(nextNode);
			}
		}
	}
	
	for (int i = 0; i < nodesCount; i++)
		maxDistance[i] = getMaxDistance(adjList, nodesCount, i, maxSteps);

	int result = -1;

	for (int i = 0; i < nodesCount; i++)
		if (maxDistance[i] != -1) {
			int currRes = ceil((double) dist[i] / (double) maxSteps);
			currRes = maxSteps * currRes + maxDistance[i];
			
			if (result == -1 || result > currRes)
				result = currRes;
		}
	
	return result;
}

// 6. Merges two sequences of ranges from two arrays - O(n)
void processRange(int ranges[], int i, int &openedCount, int &currentStart, vector<int> &result)
{
	if (i%2 == 0) {
		if (openedCount == 0)
			currentStart = ranges[i];	
		openedCount++;
	}
	else {
		if (openedCount == 1) {
			if (!result.empty() && result.back() == currentStart)
				result.back() = ranges[i];
			else {
				result.push_back(currentStart);
				result.push_back(ranges[i]);
			}
		}
		openedCount--;
	}
}

vector<int> mergeRanges(int rangesA[], int sizeA, int rangesB[], int sizeB)
{
	vector<int> result;
	int openedCount = 0, i = 0, j = 0, currentStart = 0;
	
	while (i < sizeA || j < sizeB) {
		if (j == sizeB || rangesA[i] <= rangesB[j]) {
			processRange(rangesA, i, openedCount, currentStart, result);
			i++;
		}
		else {
			processRange(rangesB, j, openedCount, currentStart, result);
			j++;
		}
	}
	
	return result;
}

// 7. Rotates a matrix 90 degrees clockwise in place - O(n^2)
void rotateLayer(int** matrix, int layer, int size)
{
	int firstInd = layer, lastInd = size - layer - 1;
	
	for (int i = 0; i < lastInd - firstInd; i++) {
		int tmp = matrix[firstInd][firstInd + i];
		matrix[firstInd][firstInd + i] = matrix[lastInd - i][firstInd];
		matrix[lastInd - i][firstInd] = matrix[lastInd][lastInd - i];
		matrix[lastInd][lastInd - i] = matrix[firstInd + i][lastInd];
		matrix[firstInd + i][lastInd] = tmp;
	}
}

void rotateMatrix(int** matrix, int size)
{
	for (int i = 0; i < size/2; i++)
		rotateLayer(matrix, i, size);
}

// 8. Adds two bigInts.
int digitToInt(char digit)
{
	return (int)digit - 48;
}

char intToDigit(int number)
{
	return (char)(number + 48);
}

string addBigInts(const string &first, const string &second)
{
	string result;
	result.resize(max(first.size(), second.size()) + 1);
	
	int i = first.size() - 1, j = second.size() - 1, k = result.size() - 1;
	int buffer = 0;

	while (k >= 1) {
		int firstNumber = (i < 0) ? 0 : digitToInt(first[i]);
		int secondNumber = (j < 0) ? 0 : digitToInt(second[j]);
		
		int sum = firstNumber + secondNumber + buffer;
		result[k] = intToDigit(sum % 10);
		
		if (sum >= 10)
			buffer = 1;
		else
			buffer = 0;
		
		i--, j--, k--;
	}

	result[0] = intToDigit(buffer);

	if (result[0] == '0')
		return result.substr(1);
	else
		return result;
}

// 9. Returns the name of an int.
string numbers[] = {"zero", "one", "two", "three", "four", "five",
					"six", "seven", "eight", "nine", "ten", "eleven",
					"twelve", "thirteen", "fourteen", "fifteen",
					"sixteen", "seventeen", "eighteen", "nineteen"};
string tens[] = {"zero", "ten", "twenty", "thirty", "forty", "fifty",
				 "sixty", "seventy", "eighty", "ninety"};
string thousands[] = {"", "thousand", "million", "billion", "trillion"};

string getTensName(int number)
{
	if (number < 20)
		return numbers[number];
	else if (number % 10 == 0)
		return tens[number/10];
	else
		return tens[number/10] + "-" + numbers[number%10];
}

string getShortName(int number)
{
	string tensResult = getTensName(number % 100);
	
	if (number < 100)
		return tensResult;
	else if (number % 100 == 0)
		return numbers[number/100] + " hundred";
	else
		return numbers[number/100] + " hundred " + tensResult;
}

int getDigitsCount(int num)
{
	int result = 1, number = num;
	
	while (number >= 10) {
		number /= 10;
		result++;
	}
	
	return result;
}

int pow1000(int n)
{
	int res = 1;
	while (n > 0) {
		res *= 1000;
		n--;
	}

	return res;
}

int getShortNumber(int number, int level)
{
	return (number / pow1000(level)) % 1000;
}

string getNumberName(int number)
{
	if (number == 0)
		return "zero";
	
	int sizeLevel = (getDigitsCount(number) - 1)/3;
	string res;
	
	for (int level = sizeLevel; level >= 0; level--) {
		string shortName = getShortName(getShortNumber(number, level));
		
		if (shortName != "zero")
			res += shortName + " " + thousands[level] + " ";
	}

	res.pop_back();
	return res;
}

// 10. Returns the number of occurences of the pattern in the text (KMP) - O(n)
char getChar(const string &pattern, const string &text, int pos)
{
	if (pos < (int) pattern.size())
		return pattern[pos];
	else if (pos == (int) pattern.size())
		return '#';
	else
		return text[pos - pattern.size() - 1];
}

int countOccurences(const string &pattern, const string &text)
{
	int prefSize = pattern.size() + text.size() + 1;
	int pref[prefSize];
	pref[0] = 0;
	
	for (int i = 1, j = 0; i < prefSize; i++) {
		while (j > 0 && getChar(pattern, text, j) != getChar(pattern, text, i))
			j = pref[j-1];
		
		if (getChar(pattern, text, j) != getChar(pattern, text, i))
			pref[i] = 0;
		else {
			j++;
			pref[i] = j;
		}
	}
	
	int res = 0;
	for (int i = 0; i < prefSize; i++)
		if (pref[i] == (int) pattern.size())
			res++;
	
	return res;
}

// 11. Returns the best possible result in the "boxes game" - O(n^2)
struct Score
{
	int firstPlayer;
	int secondPlayer;
};

int getScore(int boxes[], int boxesCount)
{
	Score scores[boxesCount][boxesCount];
	
	for (int i = 0; i < boxesCount; i++) {
		scores[i][i].firstPlayer = boxes[i];
		scores[i][i].secondPlayer = 0;
	}

	for (int i = 2; i <= boxesCount; i++)
		for (int begin = 0, end = i - 1; end < boxesCount; begin++, end++) {
			int beginScore = boxes[begin] + scores[begin + 1][end].secondPlayer;
			int endScore = boxes[end] + scores[begin][end - 1].secondPlayer;
			
			if (beginScore >= endScore) {
				scores[begin][end].firstPlayer = beginScore;
				scores[begin][end].secondPlayer = scores[begin + 1][end].firstPlayer;
			}
			else {
				scores[begin][end].firstPlayer = endScore;
				scores[begin][end].secondPlayer = scores[begin][end - 1].firstPlayer;
			}
		}
	
	return scores[0][boxesCount - 1].firstPlayer;
}

// 12. Given a set of points on a plane, checks if there're three that are on the same line - O(n^2 log n)
struct Point
{
	double x;
	double y;
};

bool operator<(const Point &a, const Point &b)
{
	if (a.y != b.y)
		return a.y < b.y;
	else
		return a.x < b.x;
}

struct Line
{
	Point pointA;
	Point pointB;
};

double getCos(Line line)
{
	double distX = line.pointB.x - line.pointA.x;
	double distY = line.pointB.y - line.pointA.y;
	double length = sqrt(distX * distX + distY * distY);

	return distX / length;
}

Line makeLine(Point a, Point b)
{
	Line line;
	
	if ((a.y == b.y && a.x < b.x) || a.y < b.y) {
		line.pointA = a;
		line.pointB = b;
	}
	else {
		line.pointA = b;
		line.pointB = a;
	}
	
	return line;
}

bool cmp(const Line &a, const Line &b)
{
	return getCos(a) < getCos(b);
}

bool isSthCollinear(Point points[], int pointsCount)
{
	vector<Line> lines;
	
	for (int i = 0; i < pointsCount - 1; i++)
		for (int j = i+1; j < pointsCount; j++)
			lines.push_back(makeLine(points[i], points[j]));
	
	sort(lines.begin(), lines.end(), cmp);
	
	int rangeBegin = 0, rangeEnd = 0;
	set<Point> pointsOccured;
	
	while (true) {
		while (rangeEnd < (int) lines.size() - 1 && getCos(lines[rangeEnd + 1]) == getCos(lines[rangeBegin]))
			rangeEnd++;

		for (int i = rangeBegin; i <= rangeEnd; i++) {
			Point pts[] = {lines[i].pointA, lines[i].pointB};
			
			for (int j = 0; j < 2; j++) {
				if (pointsOccured.find(pts[j]) == pointsOccured.end())
					pointsOccured.insert(pts[j]);
				else
					return true;
			}
		}

		pointsOccured.clear();

		if (rangeEnd == (int) lines.size() - 1)
			break;

		rangeBegin = rangeEnd + 1;
		rangeEnd++;
	}

	return false;
}

// 13. Returns minimum distance from a word to another word from a dictionary - O(n^2) (there's a faster way)
bool isSimilar(const string &a, const string &b)
{
	int diffsCount = 0;
	
	for (unsigned int i = 0; i < a.size(); i++)
		if (a[i] != b[i]) {
			diffsCount++;
		
		if (diffsCount > 1)
			return false;
	}
	
	return diffsCount == 1;
}

vector<string> findShortestPath(string startWord, string endWord, string dict[], int dictSize)
{
	vector<int> adjList[dictSize];
	int startIndex, endIndex;
	int reachedFrom[dictSize];
	
	for (int i = 0; i < dictSize; i++) {
		if (dict[i] == startWord)
			startIndex = i;
		else if (dict[i] == endWord)
			endIndex = i;
	}
	
	for (int i = 0; i < dictSize - 1; i++)
		for (int j = i + 1; j < dictSize; j++)
			if (isSimilar(dict[i], dict[j])) {
				adjList[i].push_back(j);
				adjList[j].push_back(i);
			}
	
	bool visited[dictSize];
	int dist[dictSize];
	queue<int> nodesQueue;
	
	fill(visited, visited + dictSize, false);
	fill(dist, dist + dictSize, -1);
	
	visited[startIndex] = true;
	dist[startIndex] = 0;
	reachedFrom[startIndex] = -1;
	nodesQueue.push(startIndex);
	
	while (!nodesQueue.empty()) {
		int node = nodesQueue.front();
		nodesQueue.pop();
		
		if (node == endIndex)
			break;
		
		for (unsigned int i = 0; i < adjList[node].size(); i++) {
			int nextNode = adjList[node][i];
			
			if (!visited[nextNode]) {
				visited[nextNode] = true;
				dist[nextNode] = dist[node] + 1;
				reachedFrom[nextNode] = node;
				nodesQueue.push(nextNode);
			}
		}
	}

	vector<string> result;
	int node = endIndex;

	while (node != startIndex) {
		result.push_back(dict[node]);
		node = reachedFrom[node];
	}

	result.push_back(dict[startIndex]);
	reverse(result.begin(), result.end());

	return result;
}

// 14. Traverses a binary tree in-order in place (without recursion) - O(n)
struct BinNode
{
	int value;
	BinNode* parent;
	BinNode* leftChild;
	BinNode* rightChild;
};

void goUp(BinNode* &node, BinNode* &lastNode)
{
	lastNode = node;
	node = node->parent;
}

void goRight(BinNode* &node, BinNode* &lastNode)
{
	printf("%d ", node->value);
	
	if (node->rightChild != NULL) {
		lastNode = node;
		node = node->rightChild;
	}
	else
		goUp(node, lastNode);
}

void goLeft(BinNode* &node, BinNode* &lastNode)
{
	if (node->leftChild != NULL) {
		lastNode = node;
		node = node->leftChild;
	}
	else
		goRight(node, lastNode);
}

void inOrderInPlace(BinNode* root)
{
	BinNode* node = root;
	BinNode* lastNode = NULL;
	
	while (node != NULL) {
		if (lastNode == node->parent)
			goLeft(node, lastNode);
		else if (lastNode == node->leftChild)
			goRight(node, lastNode);
		else if (lastNode == node->rightChild)
			goUp(node, lastNode);
	}

	printf("\n");
}

BinNode* arrayToBinNodeRec(int array[], int n, int i)
{
	if (i >= n || array[i] == 0)
		return NULL;
	
	BinNode* node = new BinNode;
	node->value = array[i];
	node->parent = NULL;
	node->leftChild = arrayToBinNodeRec(array, n, 2*(i+1)-1);
	node->rightChild = arrayToBinNodeRec(array, n, 2*(i+1));
	
	if (node->leftChild != NULL)
		node->leftChild->parent = node;
	if (node->rightChild != NULL)
		node->rightChild->parent = node;
	
	return node;
}

BinNode* arrayToBinNode(int array[], int n)
{
	return arrayToBinNodeRec(array, n, 0);
}

// 15. Traverses a tree pre-order without recursion - O(n)
struct TNode
{
	int value;
	vector<TNode*> children;
};

struct StackEntry
{
	TNode* node;
	int nextChild;
};

StackEntry makeEntry(TNode* n, int child)
{
	StackEntry res;
	res.node = n;
	res.nextChild = child;
	return res;
}

void preOrderIteratively(TNode* root)
{
	stack<StackEntry> nodesStack;
	nodesStack.push(makeEntry(root, 0));
	
	while (!nodesStack.empty()) {
		TNode* &node = nodesStack.top().node;
		int &nextChild = nodesStack.top().nextChild;
		
		if (nextChild == 0)
			printf("%d ", node->value);
		
		if (nextChild < (int) node->children.size())
			nodesStack.push(makeEntry(node->children[nextChild], 0));
		else {
			nodesStack.pop();
			if (!nodesStack.empty())
				nodesStack.top().nextChild++;
		}
	}
	
	printf("\n");
}

TNode* arrayToTNodeRec(int array[], int n, int i)
{
	if (i >= n || array[i] == 0)
		return NULL;
	
	TNode* node = new TNode;
	node->value = array[i];
	TNode* left = arrayToTNodeRec(array, n, 2*(i+1)-1);
	TNode* right = arrayToTNodeRec(array, n, 2*(i+1));
	
	if (left != NULL)
		node->children.push_back(left);
	if (right != NULL)
		node->children.push_back(right);
	
	return node;
}

TNode* arrayToTNode(int array[], int n)
{
	return arrayToTNodeRec(array, n, 0);
}

// 16. Checks if a singly linked list of ints is a palindrome - O(n)
struct SLNode
{
	int value;
	SLNode* next;
};

SLNode* makeNode(int val, SLNode* n)
{
	SLNode* node = new SLNode();
	node->value = val;
	node->next = n;
	return node;
}

void deleteList(SLNode* list)
{
	while (list != NULL) {
		SLNode* tmp = list->next;
		delete list;
		list = tmp;
	}
}

bool isPalindrome(SLNode* root)
{
	SLNode* list = root;
	SLNode* revList = NULL;

	while (list != NULL) {
		revList = makeNode(list->value, revList);
		list = list->next;
	}	

	list = root;
	SLNode* fullRevList = revList;

	while (list != NULL) {
		if (list->value != revList->value) {
			deleteList(fullRevList);
			return false;
		}
		
		list = list->next;
		revList = revList->next;
	}

	deleteList(fullRevList);
	return true;
}

SLNode* arrayToSLNode(int array[], int n)
{
	SLNode* res = NULL;
	
	for (int i = n-1; i >= 0; i--) {
		SLNode* node = new SLNode;
		node->value = array[i];
		node->next = res;
		res = node;
	}
	
	return res;
}

// 17. Cellular sort or something - O(n^2)
struct Result
{
	int lastFirst;
	int lastLast;
};

int countSequences(int array[], int size)
{
	Result res[size][size];
	
	if (size == 1)
		return 1;
	
	for (int i = 0; i < size - 1; i++) {
		if (array[i] < array[i + 1])
			res[i][i + 1].lastFirst = res[i][i + 1].lastLast = 1;
		else
			res[i][i + 1].lastFirst = res[i][i + 1].lastLast = 0;
	}

	for (int i = 3; i <= size; i++)
		for (int begin = 0, end = i - 1; end < size; begin++, end++) {
			res[begin][end].lastFirst = res[begin][end].lastLast = 0;
			
			if (array[end] > array[begin])
				res[begin][end].lastLast += res[begin][end - 1].lastFirst;
			if (array[end] > array[end - 1])
				res[begin][end].lastLast += res[begin][end - 1].lastLast;
			if (array[begin] < array[begin + 1])
				res[begin][end].lastFirst += res[begin + 1][end].lastFirst;
			if (array[begin] < array[end])
				res[begin][end].lastFirst += res[begin + 1][end].lastLast;
		}
	
	return res[0][size - 1].lastFirst + res[0][size - 1].lastLast;
}

// 18. Switching between two different representations of a tree - O(n)
struct ListNode
{
	int value;
	list<ListNode*> children;
};

struct TreeNode
{
	int value;
	TreeNode* brother;
	TreeNode* son;
};

ListNode* makeListNode(int val, list<ListNode*> sons)
{
	ListNode* res = new ListNode;
	res->value = val;
	res->children = sons;
	return res;
}

TreeNode* makeTreeNode(int val, TreeNode* bro, TreeNode* s)
{
	TreeNode* res = new TreeNode;
	res->value = val;
	res->brother = bro;
	res->son = s;
	return res;
}

TreeNode* makeTreeFromList(ListNode* listNode)
{
	TreeNode* child = NULL;
	
	for (auto it = listNode->children.rbegin(); it != listNode->children.rend(); ++it) {
		TreeNode* res = makeTreeFromList(*it);
		res->brother = child;
		child = res;
	}

	return makeTreeNode(listNode->value, NULL, child);
}

ListNode* makeListFromTree(TreeNode* treeNode)
{
	TreeNode* child = treeNode->son;
	list<ListNode*> children;

	while (child != NULL) {
		children.push_back(makeListFromTree(child));
		child = child->brother;
	}

	return makeListNode(treeNode->value, children);
}

// 19. Generates all permutations of an int array - O(n!)
void printList(list<int> &l)
{
	for (auto it = l.begin(); it != l.end(); ++it)
		printf("%d ", *it);
	printf("\n");
}

void generatePermutations(int i, int numbers[], int size, list<int> &usedNumbers)
{
	if (i == size)
		printList(usedNumbers);
	else {
		for (auto it = usedNumbers.begin(); it != usedNumbers.end(); ++it) {
			auto newIt = usedNumbers.insert(it, numbers[i]);
			generatePermutations(i + 1, numbers, size, usedNumbers);
			usedNumbers.erase(newIt);
		}

		usedNumbers.push_back(numbers[i]);
		generatePermutations(i + 1, numbers, size, usedNumbers);
		usedNumbers.pop_back();
	}
}

void printAllPermutations(int numbers[], int size)
{
	list<int> usedNumbers;
	generatePermutations(0, numbers, size, usedNumbers);
}

void shuffleAllPermutationsRec(int array[], int size, int step)
{
	if (step == size - 1) {
		for (int i = 0; i < size; i++)
			printf("%d ", array[i]);
		printf("\n");
	}
	else {
		for (int i = step; i < size; i++) {
			swap(array[step], array[i]);
			shuffleAllPermutationsRec(array, size, step + 1);
			swap(array[step], array[i]);
		}
	}
}

void shuffleAllPermutations(int array[], int size)
{
	shuffleAllPermutationsRec(array, size, 0);
}
