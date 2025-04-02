#ifndef ALGORITHM_FNS_H
#define ALGORITHM_FNS_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <omp.h>
#include <tuple>

using namespace std;

#define INF numeric_limits<int>::max()

void parallelBellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source);
void bellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source);
void parallelFloydWarshall(vector<vector<int>> &dist, int num_threads);
void floydWarshall(vector<vector<int>> &dist);
void johnsons(int nodes, const vector<tuple<int, int, int>> &edges);
void johnsonsParallel(int nodes, const vector<tuple<int, int, int>> &edges, int numThreads);


#endif
