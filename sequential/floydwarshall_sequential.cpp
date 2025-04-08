#include <iostream>
#include <vector>
#include <limits>
#include "../algorithm_fns.h"

using namespace std;

#define INF numeric_limits<int>::max()

// Floyd-Warshall function (Sequential)
vector<vector<int>> floydWarshall(vector<vector<int>> &dist) {

    int V = dist.size();

    for (int k = 0; k < V; k++) {  // Intermediate node
        for (int i = 0; i < V; i++) {  // Source node
            for (int j = 0; j < V; j++) {  // Destination node
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
