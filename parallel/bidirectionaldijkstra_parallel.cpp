#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_set>
#include <omp.h>

using namespace std;

const int INF = numeric_limits<int>::max();

int parallelBidirectionalDijkstra(int nodes, vector<vector<pair<int, int>>> &graph, int source, int target, int num_threads) {
    vector<int> distF(nodes, INF), distB(nodes, INF);
    distF[source] = 0;
    distB[target] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pqF, pqB;
    pqF.push({0, source});
    pqB.push({0, target});

    unordered_set<int> visitedF, visitedB;
    int best = INF;

    // Reverse graph for backward search
    vector<vector<pair<int, int>>> reverseGraph(nodes);
    for (int u = 0; u < nodes; ++u) {
        for (auto &[v, w] : graph[u]) {
            reverseGraph[v].push_back({u, w});
        }
    }

    omp_set_num_threads(num_threads);  // Set the number of threads for OpenMP

    while (!pqF.empty() || !pqB.empty()) {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                if (!pqF.empty()) {
                    auto [d, u] = pqF.top(); pqF.pop();
                    bool skip = visitedF.count(u);
                    if (!skip) {
                        visitedF.insert(u);
                        if (visitedB.count(u)) {
                            #pragma omp critical(best)
                            best = min(best, distF[u] + distB[u]);
                        }
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
            }

            #pragma omp section
            {
                if (!pqB.empty()) {
                    auto [d, u] = pqB.top(); pqB.pop();
                    bool skip = visitedB.count(u);
                    if (!skip) {
                        visitedB.insert(u);
                        if (visitedF.count(u)) {
                            #pragma omp critical(best)
                            best = min(best, distF[u] + distB[u]);
                        }
                        for (auto &[v, w] : reverseGraph[u]) {
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
        } }


    if (best == INF) {
        cout << "No path found between " << source << " and " << target << endl;
    } else {
	cout << "Shortest path between " << source << " and " << target << ": " << best << endl;
    }


    return best;
}
