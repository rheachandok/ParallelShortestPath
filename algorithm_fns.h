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

vector<int> parallelBellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source, int num_threads);
vector<int> bellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source);
vector<vector<int>> parallelFloydWarshall(vector<vector<int>> &dist, int num_threads);
vector<vector<int>> floydWarshall(vector<vector<int>> &dist);
vector<vector<int>> johnsons(int nodes, const vector<tuple<int, int, int>> &edges);
vector<vector<int>> johnsonsParallel(int nodes, const vector<tuple<int, int, int>> &edges, int numThreads);
vector<int> dijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source);
vector<int> parallelDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int num_threads);
int bidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int target);
int parallelBidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int target, int num_threads);


#endif
