#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include "../algorithm_fns.h"

using namespace std;

vector<int> parallelDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int num_threads) {
    vector<int> dist(nodes, numeric_limits<int>::max());
    vector<bool> visited(nodes, false);
    dist[source] = 0;

    omp_set_num_threads(num_threads);

    for (int i = 0; i < nodes; i++) {
        int u = -1;
        int minDist = numeric_limits<int>::max();

        // Step 1: Parallel reduction to find node with the minimum dist
        #pragma omp parallel
        {
            int local_u = -1;
            int local_minDist = numeric_limits<int>::max();

            #pragma omp for nowait
            for (int v = 0; v < nodes; v++) {
                if (!visited[v] && dist[v] < local_minDist) {
                    local_minDist = dist[v];
                    local_u = v;
                }
            }

            // Use critical to safely compare and update u and minDist
            #pragma omp critical
            {
                if (local_minDist < minDist) {
                    minDist = local_minDist;
                    u = local_u;
                }
            }
        }

        if (u == -1) break;
        visited[u] = true;

        // Step 2: Parallel relaxation of neighbors
        #pragma omp parallel for
        for (int j = 0; j < graph[u].size(); j++) {
            int v = graph[u][j].first;
            int weight = graph[u][j].second;
            int potentialDist = dist[u] + weight;

            #pragma omp critical
            {
                if (!visited[v] && potentialDist < dist[v]) {
                    dist[v] = potentialDist;
                }
            }
        }
    }

    return dist;
}
