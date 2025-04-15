#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

// Parallel Floyd-Warshall function
vector<vector<int>> parallelFloydWarshall(vector<vector<int>>& dist, int num_threads) {
    const int V = static_cast<int>(dist.size());
    omp_set_num_threads(num_threads);

    for (int k = 0; k < V; ++k) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == INF) continue;
            int dik = dist[i][k];

            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == INF) continue;
                int alt = dik + dist[k][j];
                if (alt < dist[i][j]) {
                    dist[i][j] = alt;
                }
            }
        }
    }

    cout << "Shortest distances between every pair of vertices:\n";
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                cout << "INF\t";
            else
                cout << dist[i][j] << "\t";
        }
        cout << endl;
    }

    return dist;
}
