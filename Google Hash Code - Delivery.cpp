// Google Hash Code 2016 - Delivery
// Uses 1 warehouse only
// Score: 115010

#include <cstdio>
#include <vector>
#include <deque>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

int rowsCount, colsCount, dronesCount, maxTime, maxLoad;
int productsCount, productWeights[2500];
int warehousesCount, ordersCount;

struct Warehouse
{
	int row, col;
	int productItems[2500];
};

struct cmp
{
	bool operator()(const int &a, const int &b)
	{
		if (productWeights[a] != productWeights[b])
			return productWeights[a] < productWeights[b];
		return a < b;
	}
};

struct Order
{
	int orderId;
	int row, col;
	int itemsCount;
	map<int, int, cmp> items;
};

struct Drone
{
	int row, col;
	int timeUsed;
	map<int, int, cmp> items;
};

struct Command
{
	int drone;
	char command;
	int dest;
	int productType;
	int itemsCount;
};

Warehouse warehouses[20];
Drone drones[30];
deque<Order> orders;
vector<Command> commands;

void getInput()
{
	scanf("%d%d%d%d%d", &rowsCount, &colsCount, &dronesCount, &maxTime, &maxLoad);
	
	scanf("%d", &productsCount);
	for (int i = 0; i < productsCount; i++)
		scanf("%d", &productWeights[i]);
	
	scanf("%d", &warehousesCount);
	for (int i = 0; i < warehousesCount; i++) {
		scanf("%d%d", &warehouses[i].row, &warehouses[i].col);
		
		for (int j = 0; j < productsCount; j++)
			scanf("%d", &warehouses[i].productItems[j]);
	}
	
	scanf("%d", &ordersCount);
	for (int i = 0; i < ordersCount; i++) {
		Order order;
		
		order.orderId = i;
		scanf("%d%d%d", &order.row, &order.col, &order.itemsCount);
		
		for (int j = 0, item; j < order.itemsCount; j++) {
			scanf("%d", &item);
			
			if (order.items.find(item) == order.items.end())
				order.items[item] = 1;
			else
				order.items[item]++;
		}
		
		orders.push_back(order);
	}
}

int getDistance(int rowA, int colA, int rowB, int colB)
{
	return ceil(sqrt( (rowA-rowB)*(rowA-rowB) + (colA-colB)*(colA-colB) ));
}

int getOrderCost(const Order &order)
{
	return order.itemsCount * (2*getDistance(order.row, order.col, warehouses[0].row, warehouses[0].col) + 2);
}

bool ordersComp(const Order &a, const Order &b)
{
	if (getOrderCost(a) != getOrderCost(b))
		return getOrderCost(a) < getOrderCost(b);
	return a.orderId < b.orderId;
}

int getFirstDrone()
{
	int houseRow = warehouses[0].row, houseCol = warehouses[0].col;	
	int dist = getDistance(drones[0].row, drones[0].col, houseRow, houseCol);
	int minTime = drones[0].timeUsed + dist;
	int drone = 0;
	
	for (int i = 1; i < dronesCount; i++) {
		dist = getDistance(drones[i].row, drones[i].col, houseRow, houseCol);
		
		if (drones[i].timeUsed + dist < minTime) {
			minTime = drones[i].timeUsed + dist;
			drone = i;
		}
	}
	
	return drone;
}

bool checkOrder(Order &order)
{
	for (auto it = order.items.begin(); it != order.items.end(); ++it)
		if (warehouses[0].productItems[it->first] < it->second)
			return false;
	
	return true;
}

int getItemsToLoad(Order &order)
{
	int items = 0, loadLeft = maxLoad;
	
	for (auto it = order.items.begin(); it != order.items.end(); ++it) {
		if (productWeights[it->first] * it->second <= loadLeft) {
			items += it->second;
			loadLeft -= productWeights[it->first] * it->second;
		}
		else
			return items + loadLeft / productWeights[it->first];
	}
	
	return items;
}

Command makeCommand(int drone, char command, int dest, int productType, int itemsCount)
{
	Command comm;
	comm.drone = drone, comm.command = command, comm.dest = dest;
	comm.productType = productType, comm.itemsCount = itemsCount;
	return comm;
}

bool checkCommandTime(int destRow, int destCol, Drone &drone)
{
	int timeNeeded = getDistance(drone.row, drone.col, destRow, destCol) + 1;
	return drone.timeUsed + timeNeeded <= maxTime;
}

void doCommand(Command comm, int destRow, int destCol, Drone &drone)
{
	drone.timeUsed += getDistance(drone.row, drone.col, destRow, destCol) + 1;
	drone.row = destRow;
	drone.col = destCol;
	commands.push_back(comm);
}

int main()
{
	getInput();
	sort(orders.begin(), orders.end(), ordersComp);
	
	for (int i = 0; i < dronesCount; i++) {
		drones[i].row = warehouses[0].row;
		drones[i].col = warehouses[0].col;
		drones[i].timeUsed = 0;
	}
	
	int houseRow = warehouses[0].row, houseCol = warehouses[0].col;
	
	while (!orders.empty()) {
		Order &order = orders.front();
		int droneId = getFirstDrone();
		Drone &drone = drones[droneId];
		
		if (!checkOrder(order)) {
			orders.pop_front();
			if (orders.empty())
				break;
			else
				continue;
		}
		
		int itemsToLoad = getItemsToLoad(order);
		bool timeIsUp = false;
		
		while (itemsToLoad > 0) {
			auto it = order.items.begin();
			
			if (!checkCommandTime(houseRow, houseCol, drone)) {
				timeIsUp = true;
				break;
			}
			
			int itemsLoaded = min(itemsToLoad, it->second);
			Command comm = makeCommand(droneId, 'L', 0, it->first, itemsLoaded);
			doCommand(comm, houseRow, houseCol, drone);
			drone.items[it->first] = itemsLoaded;
			warehouses[0].productItems[it->first] -= itemsLoaded;
			
			if (itemsLoaded == it->second)
				order.items.erase(order.items.begin());
			else
				order.items.begin()->second -= itemsLoaded;
			
			itemsToLoad -= itemsLoaded;
		}
		
		if (timeIsUp)
			break;
		
		for (auto it = drone.items.begin(); it != drone.items.end(); ++it) {
			if (!checkCommandTime(order.row, order.col, drone)) {
				timeIsUp = true;
				break;
			}
			
			Command comm = makeCommand(droneId, 'D', order.orderId, it->first, it->second);
			doCommand(comm, order.row, order.col, drone);
		}
		
		if (timeIsUp)
			break;
		
		drone.items.clear();
		
		if (order.items.empty())
			orders.pop_front();
	}
	
	printf("%lu\n", commands.size());
	for (unsigned int i = 0; i < commands.size(); i++) {
		Command &comm = commands[i];
		printf("%d %c %d %d %d\n", comm.drone, comm.command, comm.dest, comm.productType, comm.itemsCount);
	}
	
	return 0;
}
