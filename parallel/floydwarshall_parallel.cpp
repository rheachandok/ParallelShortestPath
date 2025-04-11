#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()
inline int idx(int i, int j, int V) { return i*V + j; }
// Parallel Floyd-Warshall function
vector<vector<int>>
parallelFloydWarshall(vector<vector<int>>& dist, int num_threads){
    const int V = static_cast<int>(dist.size());
    omp_set_num_threads(num_threads);

    #pragma omp parallel                      
    for (int k = 0; k < V; ++k) {
        #pragma omp for schedule(static)     
        for (int i = 0; i < V; ++i) {
            const int dik = dist[i][k];       
            if (dik == INF) continue;

            for (int j = 0; j < V; ++j) {
                const int alt = dik + dist[k][j];
                if (alt < dist[i][j]) dist[i][j] = alt;
            }
        }
    }
    return dist;
}

