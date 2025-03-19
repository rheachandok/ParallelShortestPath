#include <iostream>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
#include <omp.h>
using namespace std;

void bellman_ford(int p, int n, int *mat, int *dist, bool *has_negative_cycle) {
    *has_negative_cycle = false;

    omp_set_num_threads(p);

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[0] = 0;

    int *enqueue_counter = (int *)calloc(n, sizeof(int));
    bool *in_queue = (bool *)calloc(n, sizeof(bool));
    queue<int> queue;

    in_queue[0] = true;
    queue.push(0);
    enqueue_counter[0] = 1;

    while (!queue.empty() && !*has_negative_cycle) {
        int u = queue.front();
        queue.pop();
        in_queue[u] = false;

#pragma omp parallel for
        for (int v = 0; v < n; v++) {
            int weight = mat[u * n + v];
            if (weight < INF) {
                int new_dist = weight + dist[u];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    enqueue_counter[v]++;
                    if (!in_queue[v]) {
                        in_queue[v] = true;
                        if (enqueue_counter[v] >= n) {
                            *has_negative_cycle = true;
                        }
#pragma omp critical
                        {
                            queue.push(v);
                        }
                    }
                }
            }
        }
    }

    free(enqueue_counter);
    free(in_queue);
}
