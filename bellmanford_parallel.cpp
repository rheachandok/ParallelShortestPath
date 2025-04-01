#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <omp.h>
#include "algorithm_fns.h"

using namespace std;

// Parallel Bellman-Ford function with configurable number of threads
void parallelBellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source, int num_threads) {
    vector<int> dist(nodes, numeric_limits<int>::max()); // Distance array, initialized to INF
    dist[source] = 0; // Distance to source is 0

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Relax all edges |V|-1 times
    for (int i = 0; i < nodes - 1; i++) {
        #pragma omp parallel for
        for (int u = 0; u < nodes; u++) {
            for (auto edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                    #pragma omp critical
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    // Check for negative weight cycles
    bool hasNegativeCycle = false;
    #pragma omp parallel for
    for (int u = 0; u < nodes; u++) {
        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                hasNegativeCycle = true;
            }
        }
    }

    if (hasNegativeCycle) {
        cout << "Graph contains a negative weight cycle!\n";
        return;
    }

    // Print shortest distances
    cout << "Shortest distances from source node " << source << ":\n";
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == numeric_limits<int>::max())
            cout << "Node " << i << " -> INF\n";
        else
            cout << "Node " << i << " -> " << dist[i] << "\n";
    }
}
