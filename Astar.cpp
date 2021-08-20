#include <vector>
#include <iostream>
#include "Astar.h"
using namespace std;
int main() {
	vector<vector<int>> maze = {
		{1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,1,1,0,1,0,0,0,0,1},
		{1,0,0,1,1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,1,1,1},
		{1,1,1,0,0,0,0,0,1,1,0,1},
		{1,1,0,1,0,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,1,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1}
	};
	CGraph* graph = new CGraph(maze);
	CAStar* as = new CAStar(graph, {1, 1}, {6, 10});
	vector<pair<int, int>> path;
	path = as->GetPath();
	for (auto p : path) {
		cout << '(' << p.first << ',' << p.second << ')' << endl;
	}
	return 0;
}