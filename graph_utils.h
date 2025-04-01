#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define INF numeric_limits<int>::max()

void generateGraph(int nodes, const string &filename);
void printGraph(const vector<vector<pair<int, int>>> &adjList);
vector<vector<pair<int, int>>> loadGraph(const string &filename, int &nodes);
vector<vector<int>> convertToMatrix(const vector<vector<pair<int, int>>> &graph, int numNodes);

#endif // GRAPH_UTILS_H
