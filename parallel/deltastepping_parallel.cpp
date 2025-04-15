#include <iostream>
#include <vector>
#include <limits>
#include <set>
#include <unordered_map>
#include <omp.h>
#include <mutex>
#include "../algorithm_fns.h"

using namespace std;

vector<int> deltaSteppingParallel(int nodes, const vector<vector<pair<int, int>>> &graph, int source, int delta, int numThreads) {
    vector<int> dist(nodes, numeric_limits<int>::max());
    dist[source] = 0;

    unordered_map<int, set<int>> buckets;
    buckets[0].insert(source);

    omp_set_num_threads(numThreads);

    mutex bucket_mutex;

    int i = 0;
    while (!buckets.empty()) {
        while (buckets.count(i) && !buckets[i].empty()) {
            set<int> req;

            {
                lock_guard<mutex> lock(bucket_mutex);
                req = move(buckets[i]);
                buckets.erase(i);
            }

            vector<set<int>> local_buckets(numThreads);

            // Relax light edges in parallel
            #pragma omp parallel
            {
                int tid = omp_get_thread_num();
                for (int u : req) {
                    for (auto &[v, w] : graph[u]) {
                        if (w <= delta) {
                            int alt = dist[u] + w;
                            if (alt < dist[v]) {
                                #pragma omp critical
                                {
                                    if (alt < dist[v]) {
                                        dist[v] = alt;
                                        int b = alt / delta;
                                        local_buckets[tid].insert(v);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Merge thread-local buckets
            for (int t = 0; t < numThreads; ++t) {
                for (int v : local_buckets[t]) {
                    int b = dist[v] / delta;
                    buckets[b].insert(v);
                }
            }

            // Relax heavy edges sequentially
            for (int u : req) {
                for (auto &[v, w] : graph[u]) {
                    if (w > delta && dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        int b = dist[v] / delta;
                        buckets[b].insert(v);
                    }
                }
            }
        }

        ++i;
        while (!buckets.count(i) && i < nodes * 2) ++i;
    }


    // Print final distances
    cout << "Shortest distances from source node " << source << ":\n";
    for (int i = 0; i < nodes; ++i) {
        if (dist[i] == numeric_limits<int>::max())
            cout << "Node " << i << ": INF\n";
        else
            cout << "Node " << i << ": " << dist[i] << "\n";
    }

    return dist;
}
