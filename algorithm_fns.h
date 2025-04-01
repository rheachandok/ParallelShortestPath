#ifndef ALGORITHM_FNS_H
#define ALGORITHM_FNS_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <omp.h>

using namespace std;

#define INF numeric_limits<int>::max()

void parallelBellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source);
void bellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source);
void parallelFloydWarshall(vector<vector<int>> &dist, int num_threads);
void floydWarshall(vector<vector<int>> &dist);


#endif
