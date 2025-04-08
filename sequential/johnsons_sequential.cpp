#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

vector<vector<int>> johnsons(int nodes, const vector<tuple<int, int, int>> &edges) {
    vector<int> h(nodes, INF);
    h[nodes - 1] = 0;

    // Bellman-Ford to find potential values (h)
    for (int i = 0; i < nodes - 1; i++) {
        for (const auto &[u, v, weight] : edges) {
            if (h[u] != INF && h[u] + weight < h[v]) {
                h[v] = h[u] + weight;
            }
        }
    }

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
