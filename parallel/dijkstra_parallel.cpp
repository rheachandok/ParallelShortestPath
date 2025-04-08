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

        // Parallel section to find node with minimum distance
        #pragma omp parallel
        {
            int local_min = numeric_limits<int>::max();
            int local_u = -1;

            #pragma omp for nowait
            for (int v = 0; v < nodes; v++) {
                if (!visited[v] && dist[v] < local_min) {
                    local_min = dist[v];
                    local_u = v;
                }
            }

            #pragma omp critical
            {
                if (local_min < minDist) {
                    minDist = local_min;
                    u = local_u;
                }
            }
        }

        if (u == -1) break; // All nodes reachable visited

        visited[u] = true;

        // Update neighbors
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

    
    cout << "Parallel Dijkstra shortest distances from node " << source << ":\n";
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == numeric_limits<int>::max())
            cout << "Node " << i << " -> INF\n";
        else
            cout << "Node " << i << " -> " << dist[i] << "\n";
    }
    

    return dist;
}

