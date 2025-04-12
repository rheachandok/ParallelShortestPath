#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <omp.h>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

vector<vector<int>> johnsonsParallel(int nodes, const vector<tuple<int, int, int>> &edges, int num_threads) {
    omp_set_num_threads(num_threads);

    // Step 1: Bellman-Ford with virtual node
    int bellmanNodes = nodes + 1;
    vector<int> h(bellmanNodes, INF);
    h[nodes] = 0;  // Virtual node at index 'nodes'

    vector<tuple<int, int, int>> bellmanEdges = edges;
    for(int i = 0; i < nodes; i++) {
        bellmanEdges.emplace_back(nodes, i, 0);
    }

    // Parallel Bellman-Ford implementation
    vector<int> h_old = h;
    for(int i = 0; i < bellmanNodes - 1; i++) {
        vector<int> h_new = h_old;
        bool changed = false;

        #pragma omp parallel for reduction(||:changed)
        for(size_t j = 0; j < bellmanEdges.size(); j++) {
            auto [u, v, weight] = bellmanEdges[j];
            if(h_old[u] != INF && h_old[u] + weight < h_new[v]) {
                #pragma omp critical
                {
                    if(h_old[u] + weight < h_new[v]) {
                        h_new[v] = h_old[u] + weight;
                        changed = true;
                    }
                }
            }
        }
        
        if(!changed) break;
        h_old = h_new;
    }

    // Negative cycle detection
    bool hasNegativeCycle = false;
    #pragma omp parallel for
    for(size_t j = 0; j < bellmanEdges.size(); j++) {
        auto [u, v, weight] = bellmanEdges[j];
        if(h_old[u] != INF && h_old[u] + weight < h_old[v]) {
            #pragma omp critical
            hasNegativeCycle = true;
        }
    }

    if(hasNegativeCycle) {
        cerr << "Graph contains negative weight cycle!" << endl;
        return {};
    }

    // Remove virtual node's potential
    h_old.pop_back();
    h = h_old;

    // Step 2: Reweight edges
    vector<vector<pair<int, int>>> adj(nodes);
    #pragma omp parallel for
    for(size_t j = 0; j < edges.size(); j++) {
        auto [u, v, weight] = edges[j];
        if(h[u] != INF && h[v] != INF) {
            int new_weight = weight + h[u] - h[v];
            #pragma omp critical
            adj[u].emplace_back(v, new_weight);
        }
    }

    // Step 3: Parallel Dijkstra
    vector<vector<int>> dist(nodes, vector<int>(nodes, INF));

    #pragma omp parallel for
    for(int src = 0; src < nodes; src++) {
        vector<int> minDist(nodes, INF);
        vector<bool> visited(nodes, false);
        minDist[src] = 0;

        for(int i = 0; i < nodes; i++) {
            // Find minimum unvisited node
            int u = -1;
            int current_min = INF;
            for(int j = 0; j < nodes; j++) {
                if(!visited[j] && minDist[j] < current_min) {
                    current_min = minDist[j];
                    u = j;
                }
            }

            if(u == -1 || current_min == INF) break;
            visited[u] = true;

            // Relax edges
            for(const auto& [v, weight] : adj[u]) {
                if(minDist[u] + weight < minDist[v]) {
                    minDist[v] = minDist[u] + weight;
                }
            }
        }

        // Adjust distances with potentials
        for(int i = 0; i < nodes; i++) {
            dist[src][i] = (minDist[i] == INF) 
                ? INF 
                : minDist[i] - h[src] + h[i];
        }
    }

    // Output results
    cout << "Johnson’s Algorithm (Parallel) - Shortest Distances:\n";
    for(int i = 0; i < nodes; i++) {
        for(int j = 0; j < nodes; j++) {
            if(dist[i][j] == INF) cout << "INF ";
            else cout << dist[i][j] << " ";
        }
        cout << endl;
    }

    return dist;
}
