#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <tuple>

using namespace std;

#define INF numeric_limits<int>::max()

void generateGraph(int nodes, const string &filename,  bool nonNegativeWeights, int sparsity_factor);
void printGraph(const vector<vector<pair<int, int>>> &adjList);
vector<vector<pair<int, int>>> loadGraph(const string &filename, int &nodes);
vector<vector<int>> convertToMatrix(const vector<vector<pair<int, int>>> &graph, int numNodes);
vector<tuple<int, int, int>> convertToEdgeList(const vector<vector<pair<int, int>>> &graph);

#endif // GRAPH_UTILS_H
