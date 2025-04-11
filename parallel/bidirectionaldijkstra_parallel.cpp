#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <omp.h>
#include <atomic>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

int parallelBidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph,
                                  int source, int target, int num_threads) {
    vector<int> distF(nodes, INF), distB(nodes, INF);
    vector<bool> visitedF(nodes, false), visitedB(nodes, false);

    distF[source] = 0;
    distB[target] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pqF, pqB;
    pqF.push({0, source});
    pqB.push({0, target});

    int best = INF;

    // Reverse graph for backward search
    vector<vector<pair<int, int>>> reverseGraph(nodes);
    for (int u = 0; u < nodes; ++u) {
        for (auto &[v, w] : graph[u]) {
            reverseGraph[v].push_back({u, w});
        }
    }

    omp_set_num_threads(num_threads);

    while (!pqF.empty() && !pqB.empty()) {
        // Early termination condition
        if (best <= pqF.top().first + pqB.top().first) break;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                if (!pqF.empty()) {
                    auto [d, u] = pqF.top(); pqF.pop();

                    if (!visitedF[u]) {
                        visitedF[u] = true;
                        if (visitedB[u]) {
                            #pragma omp critical
                            best = min(best, distF[u] + distB[u]);
                        }

                        for (auto &[v, w] : graph[u]) {
                            if (!visitedF[v] && distF[u] + w < distF[v]) {
                                #pragma omp critical
                                {
                                    distF[v] = distF[u] + w;
                                    pqF.push({distF[v], v});
                                }
                            }
                        }
                    }
                }
            }

            #pragma omp section
            {
                if (!pqB.empty()) {
                    auto [d, u] = pqB.top(); pqB.pop();

                    if (!visitedB[u]) {
                        visitedB[u] = true;
                        if (visitedF[u]) {
                            #pragma omp critical
                            best = min(best, distF[u] + distB[u]);
                        }

                        for (auto &[v, w] : reverseGraph[u]) {
                            if (!visitedB[v] && distB[u] + w < distB[v]) {
                                #pragma omp critical
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
    }

    if (best == INF) {
        cout << "No path found between " << source << " and " << target << endl;
        return -1;
    } else {
        cout << "Shortest path between " << source << " and " << target << ": " << best << endl;
        return best;
    }
}