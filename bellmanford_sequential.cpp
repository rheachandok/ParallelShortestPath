#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include "algorithm_fns.h"

using namespace std;

// Bellman-Ford function
void bellmanFord(int nodes, vector<vector<pair<int, int>>> &graph, int source) {
    vector<int> dist(nodes, numeric_limits<int>::max()); // Distance array, initialized to INF
    dist[source] = 0; // Distance to source is 0

    // Relax all edges |V|-1 times
    for (int i = 0; i < nodes - 1; i++) {
        for (int u = 0; u < nodes; u++) {
            for (auto edge : graph[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    // Check for negative weight cycles
    for (int u = 0; u < nodes; u++) {
        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                cout << "Graph contains a negative weight cycle!\n";
                return;
            }
        }
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