#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include "../algorithm_fns.h"

using namespace std;

const int INF = numeric_limits<int>::max();

// Bidirectional Dijkstra - Sequential
int bidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int target) {
    vector<int> distF(nodes, INF), distB(nodes, INF);
    distF[source] = 0;
    distB[target] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pqF, pqB;
    pqF.push({0, source});
    pqB.push({0, target});

    unordered_set<int> visitedF, visitedB;
    int best = INF;

    while (!pqF.empty() || !pqB.empty()) {
        if (!pqF.empty()) {
            auto [d, u] = pqF.top(); pqF.pop();
            if (visitedF.count(u)) continue;
            visitedF.insert(u);
            if (visitedB.count(u)) best = min(best, distF[u] + distB[u]);

            for (auto &[v, w] : graph[u]) {
                if (distF[u] + w < distF[v]) {
                    distF[v] = distF[u] + w;
                    pqF.push({distF[v], v});
                }
            }
        }

        if (!pqB.empty()) {
            auto [d, u] = pqB.top(); pqB.pop();
            if (visitedB.count(u)) continue;
            visitedB.insert(u);
            if (visitedF.count(u)) best = min(best, distF[u] + distB[u]);

            for (auto &[v, w] : graph[u]) {
                if (distB[u] + w < distB[v]) {
                    distB[v] = distB[u] + w;
                    pqB.push({distB[v], v});
                }
            }
        }
    }

    if (best == INF) cout << "No path found between " << source << " and " << target << endl;
    else cout << "Shortest path between " << source << " and " << target << ": " << best << endl;

    return best;
}