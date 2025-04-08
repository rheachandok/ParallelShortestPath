#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "../algorithm_fns.h"

using namespace std;

vector<int> dijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source) {
    vector<int> dist(nodes, numeric_limits<int>::max());
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        if (current_dist > dist[u])
            continue;

        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    
    cout << "Sequential Dijkstra shortest distances from node " << source << ":\n";
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == numeric_limits<int>::max())
            cout << "Node " << i << " -> INF\n";
        else
            cout << "Node " << i << " -> " << dist[i] << "\n";
    }
            

    return dist;
}

