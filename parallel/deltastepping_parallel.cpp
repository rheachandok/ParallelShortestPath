#include <iostream>
#include <vector>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <omp.h>
#include <mutex>
#include <queue>
#include <algorithm>
#include "../algorithm_fns.h"

using namespace std;

vector<int> deltaSteppingParallel(int nodes, 
    const vector<vector<pair<int, int>>>& graph, 
    int source, int delta, int numThreads) {

    vector<int> dist(nodes, numeric_limits<int>::max());
    dist[source] = 0;

    // Use vector of unordered_sets for better bucket performance
    unordered_map<int, unordered_set<int>> buckets;
    buckets[0].insert(source);

    omp_set_num_threads(numThreads);
    mutex bucket_mutex;
    vector<mutex> dist_locks(nodes);  // Fine-grained locking for distance updates

    auto relax_batch = [&](const vector<int>& vertices, int weight_limit) {
        vector<vector<pair<int, int>>> local_updates(numThreads);

        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            
            #pragma omp for schedule(dynamic, 32)  // Dynamic scheduling with chunking
            for (size_t idx = 0; idx < vertices.size(); ++idx) {
                int u = vertices[idx];
                for (auto& [v, w] : graph[u]) {
                    if (w <= weight_limit) {
                        int alt = dist[u] + w;
                        if (alt < dist[v]) {
                            local_updates[tid].emplace_back(v, alt);
                        }
                    }
                }
            }
	}

    // Apply updates in bulk with fine-grained locking
    for (int t = 0; t < numThreads; ++t) {
        for (auto& [v, alt] : local_updates[t]) {
            lock_guard<mutex> lock(dist_locks[v]);
            if (alt < dist[v]) {
                dist[v] = alt;
                int b = alt / delta;
                lock_guard<mutex> bucket_lock(bucket_mutex);
                buckets[b].insert(v);
            }
        }
    };

    int i = 0;
    while (!buckets.empty()) {
        if (buckets.count(i)) {
            vector<int> req;

            {  // Extract request nodes
                lock_guard<mutex> lock(bucket_mutex);
                if (buckets.count(i)) {
                    req.assign(buckets[i].begin(), buckets[i].end());
                    buckets.erase(i);
                }
            }

            if (!req.empty()) {
                // Process light edges in parallel
                relax_batch(req, delta);

                // Process heavy edges in parallel (same function with higher limit)
                relax_batch(req, numeric_limits<int>::max());
            }
        }

    // Find next non-empty bucket
    i++;
    if (i > nodes * 2) break;  // Safety check
        while (!buckets.count(i) && i < nodes * 2) i++;
    }

    return dist;
}

