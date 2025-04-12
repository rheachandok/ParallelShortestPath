#include <iostream>
#include <vector>
#include <limits>
#include <set>
#include <unordered_map>
#include "../algorithm_fns.h"

using namespace std;

vector<int> deltaStepping(int nodes, const vector<vector<pair<int, int>>> &graph, int source, int delta) {
    vector<int> dist(nodes, numeric_limits<int>::max());
    dist[source] = 0;

    unordered_map<int, set<int>> buckets;
    buckets[0].insert(source);

    auto relax = [&](int u, int weight_limit) {
        for (auto &[v, w] : graph[u]) {
            if (w <= weight_limit && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                int b = dist[v] / delta;
                buckets[b].insert(v);
            }
        }
    };

    int i = 0;
    while (!buckets.empty()) {
        while (buckets.count(i) && !buckets[i].empty()) {
            // Grab and clear the current bucket
            set<int> req = move(buckets[i]);
            buckets.erase(i);

            // Relax light edges (≤ delta)
            for (int u : req) {
                relax(u, delta);
            }

            // Relax heavy edges (> delta)
            for (int u : req) {
                relax(u, numeric_limits<int>::max()); // All edges
            }
        }
        ++i;
        while (!buckets.count(i) && i < nodes * 2) ++i; // Skip empty bucket indices
    }

    return dist;
}

