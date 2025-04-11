#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <omp.h>

using namespace std;

#define INF numeric_limits<int>::max()

// Standard Dijkstra using priority queue
vector<int> dijkstra(int src, const vector<vector<pair<int, int>>>& reweightedGraph) {
    int nodes = reweightedGraph.size();
    vector<int> dist(nodes, INF);
    dist[src] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto &[v, weight] : reweightedGraph[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<vector<int>> johnsonsParallel(int nodes, const vector<tuple<int, int, int>>& edges, int numThreads) {
    vector<int> h(nodes, INF);
    h[nodes - 1] = 0;

    // Step 1: Run Bellman-Ford to compute h[]
    for (int i = 0; i < nodes - 1; ++i) {
        vector<int> new_h = h;

        #pragma omp parallel for num_threads(numThreads)
        for (int j = 0; j < edges.size(); ++j) {
            int u, v, w;
            tie(u, v, w) = edges[j];
            if (h[u] != INF) {
                int candidate = h[u] + w;
                #pragma omp critical
                {
                    if (candidate < new_h[v]) {
                        new_h[v] = candidate;
                    }
                }
            }
        }

        h = new_h;
    }

    // Step 2: Reweight edges and build adjacency list
    vector<vector<pair<int, int>>> reweightedGraph(nodes);
    for (const auto& [u, v, w] : edges) {
        if (h[u] != INF && h[v] != INF) {
            int rw = w + h[u] - h[v];
            reweightedGraph[u].emplace_back(v, rw);
        }
    }

    // Step 3: Run Dijkstra for each node in parallel
    vector<vector<int>> result(nodes, vector<int>(nodes, INF));

    #pragma omp parallel for num_threads(numThreads)
    for (int src = 0; src < nodes; ++src) {
        vector<int> minDist = dijkstra(src, reweightedGraph);
        for (int dest = 0; dest < nodes; ++dest) {
            if (minDist[dest] != INF && h[src] != INF && h[dest] != INF) {
                result[src][dest] = minDist[dest] - h[src] + h[dest];
            }
        }
    }

    // Optional: Print result matrix
    cout << "Johnson’s Algorithm (Parallel) - Shortest Distances:\n";
    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            if (result[i][j] == INF)
                cout << "INF ";
            else
                cout << result[i][j] << " ";
        }
        cout << "\n";
    }

    return result;
}