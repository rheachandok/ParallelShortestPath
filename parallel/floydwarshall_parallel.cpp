#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

// Parallel Floyd-Warshall function
vector<vector<int>> parallelFloydWarshall(vector<vector<int>> &dist, int num_threads) {

    int V = dist.size();
    omp_set_num_threads(num_threads); // Set the number of threads

    for (int k = 0; k < V; k++) {
        #pragma omp parallel for collapse(2) schedule(dynamic)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
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

