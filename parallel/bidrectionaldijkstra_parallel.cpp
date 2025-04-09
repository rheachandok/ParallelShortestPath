#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <omp.h>
#include <unordered_set>
#include <unordered_map>
#include "../algorithm_fns.h"

using namespace std;

const int INF = numeric_limits<int>::max();

// Bidirectional Dijkstra - Parallel 
int parallelBidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int target, int num_threads) {
    vector<int> distF(nodes, INF), distB(nodes, INF);
    distF[source] = 0;
    distB[target] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pqF, pqB;
    pqF.push({0, source});
    pqB.push({0, target});

    unordered_set<int> visitedF, visitedB;
    int best = INF;

    omp_set_num_threads(num_threads);

    while (!pqF.empty() || !pqB.empty()) {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                if (!pqF.empty()) {
                    auto [d, u] = pqF.top(); pqF.pop();
                    if (visitedF.count(u)) return;
                    visitedF.insert(u);
                    if (visitedB.count(u)) #pragma omp critical(best) best = min(best, distF[u] + distB[u]);

                    for (auto &[v, w] : graph[u]) {
                        if (distF[u] + w < distF[v]) {
                            #pragma omp critical(distF)
                            {
                                distF[v] = distF[u] + w;
                                pqF.push({distF[v], v});
                            }
                        }
                    }
                }
            }
            #pragma omp section
            {
                if (!pqB.empty()) {
                    auto [d, u] = pqB.top(); pqB.pop();
                    if (visitedB.count(u)) return;
                    visitedB.insert(u);
                    if (visitedF.count(u)) #pragma omp critical(best) best = min(best, distF[u] + distB[u]);

                    for (auto &[v, w] : graph[u]) {
                        if (distB[u] + w < distB[v]) {
                            #pragma omp critical(distB)
                            {
                                distB[v] = distB[u] + w;
                                pqB.push({distB[v], v});
                            }
                        }
                    }
                }
            }
        }
    }

    if (best == INF) cout << "No path found between " << source << " and " << target << endl;
    else cout << "Shortest path between " << source << " and " << target << ": " << best << endl;

    return int;
}
