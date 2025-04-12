#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

vector<vector<int>> johnsons(int nodes, const vector<tuple<int, int, int>> &edges) {
    // Add virtual node and initialize Bellman-Ford
    int bellmanNodes = nodes + 1;
    vector<int> h(bellmanNodes, INF);
    h[nodes] = 0;

    vector<tuple<int, int, int>> bellmanEdges = edges;
    for (int i = 0; i < nodes; i++) {
        bellmanEdges.emplace_back(nodes, i, 0);
    }

    // Bellman-Ford with virtual node
    for (int i = 0; i < bellmanNodes - 1; i++) {
        for (const auto &[u, v, weight] : bellmanEdges) {
            if (h[u] != INF && h[u] + weight < h[v]) {
                h[v] = h[u] + weight;
            }
	}
    }

    // Negative cycle detection
    for (const auto &[u, v, weight] : bellmanEdges) {
        if (h[u] != INF && h[u] + weight < h[v]) {
            cerr << "Graph contains negative weight cycle!" << endl;
            return {};
        }
    }

    // Remove virtual node's potential
    h.pop_back();

    // Reweight edges
    vector<vector<int>> dist(nodes, vector<int>(nodes, INF));
    for (int i = 0; i < nodes; i++) dist[i][i] = 0;

    for (const auto &[u, v, weight] : edges) {
        dist[u][v] = weight + h[u] - h[v];
    }

    // Run Dijkstra for each node
    for (int src = 0; src < nodes; src++) {
        vector<int> minDist(nodes, INF);
        minDist[src] = 0;

        vector<bool> visited(nodes, false);
        for (int i = 0; i < nodes; i++) {
            int u = -1;
            for (int j = 0; j < nodes; j++) {
                if (!visited[j] && (u == -1 || minDist[j] < minDist[u])) {
                    u = j;
                }
            }

            if (minDist[u] == INF) break;
            visited[u] = true;

            for (int v = 0; v < nodes; v++) {
                if (dist[u][v] != INF && minDist[u] + dist[u][v] < minDist[v]) {
                    minDist[v] = minDist[u] + dist[u][v];
                }
            }
        }

        for (int i = 0; i < nodes; i++) {
            dist[src][i] = (minDist[i] == INF) ? INF : minDist[i] - h[src] + h[i];
        }
    }

    cout << "Johnson’s Algorithm (Sequential) - Shortest Distances:\n";
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (dist[i][j] == INF)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }
    
        

    return dist;
}
