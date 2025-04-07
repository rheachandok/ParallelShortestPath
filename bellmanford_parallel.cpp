#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <omp.h>
#include "algorithm_fns.h"

using namespace std;

void parallelBellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source, int num_threads) {
    vector<int> dist(nodes, numeric_limits<int>::max());
    vector<int> new_dist(nodes, numeric_limits<int>::max());
    dist[source] = 0;
    new_dist[source] = 0;

    omp_set_num_threads(num_threads);

    for (int i = 0; i < nodes - 1; i++) {
        bool updated = false;

        #pragma omp parallel for
        for (int u = 0; u < nodes; u++) {
            for (auto edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < new_dist[v]) {
                    #pragma omp critical
                    {
                        if (dist[u] + weight < new_dist[v]) {
                            new_dist[v] = dist[u] + weight;
                            updated = true;
                        }
                    }
                }
            }
        }

        dist = new_dist;

        if (!updated)
            break; // Early termination
    }

    // Check for negative weight cycles
    bool hasNegativeCycle = false;
    #pragma omp parallel for shared(hasNegativeCycle)
    for (int u = 0; u < nodes; u++) {
        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                #pragma omp atomic write
                hasNegativeCycle = true;
            }
        }
    }

    if (hasNegativeCycle) {
        cout << "Graph contains a negative weight cycle!\n";
        return;
    }

    // Print results
    cout << "Shortest distances from source node " << source << ":\n";
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == numeric_limits<int>::max())
            cout << "Node " << i << " -> INF\n";
        else
            cout << "Node " << i << " -> " << dist[i] << "\n";
    }
}
