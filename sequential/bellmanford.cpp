#include <iostream>
#include <vector>
#include <limits>

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

// Function to generate a random directed weighted graph
vector<vector<pair<int, int>>> generateGraph(int nodes) {
    vector<vector<pair<int, int>>> adjList(nodes);
    srand(time(0)); // Seed for random numbers

    int maxEdges = nodes * (nodes - 1); // Max edges in a directed graph (without self-loops)
    int numEdges = rand() % (maxEdges / 2) + nodes; // Ensure at least 'nodes' edges for connectivity

    for (int i = 0; i < numEdges; i++) {
        int u = rand() % nodes;  // Random source node
        int v = rand() % nodes;  // Random destination node
        int weight = rand() % 100 - 50; // Random weight (-50 to 49) to allow negative weights

        if (u != v) { // Avoid self-loops
            adjList[u].push_back({v, weight});
        }
    }

    return adjList;
}



