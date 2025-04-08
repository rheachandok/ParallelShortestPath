#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <omp.h>
#include "../utils/graph_utils.h"
#include "algorithm_fns.h"

using namespace std;

void checkEquality(vector<int>& a, vector<int>& b) {
    if (a.size() != b.size()) {
        cout << "Vectors are of different sizes. Parallel and Sequential versions don't return the same result.\n";
        return;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            cout << "Parallel and Sequential versions don't return the same result\n";
            return;
        }
    }
    cout << "Parallel and Sequential versions return the same result\n";
}

void check2dEquality(vector<vector<int>> &a, vector<vector<int>> &b)
{
    if (a.size() != b.size()) {
        cout << "2D vectors are of different sizes. Parallel and Sequential versions don't return the same result.\n";
        return;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].size() != b[i].size()) {
            cout << "Row " << i << " has different sizes in the two 2D vectors.\n";
            cout << "Parallel and Sequential versions don't return the same result\n";
            return;
        }

        for (size_t j = 0; j < a[i].size(); ++j) {
            if (a[i][j] != b[i][j]) {
                cout << "Mismatch found at row " << i << ", column " << j << ".\n";
                cout << "Parallel and Sequential versions don't return the same result\n";
                return;
            }
        }
    }

    cout << "Parallel and Sequential versions return the same result\n";
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " D/B/F/J <num_nodes> <num_threads>" << endl;
        return 1;
    }

    string algorithm = argv[1];
    int numNodes = atoi(argv[2]);
    int numThreads = atoi(argv[3]);

    // Generate and load graph
    bool pos_graph = 0;
    if(algorithm == "D")
	    pos_graph = 1;
    
    generateGraph(numNodes, "graph.txt", pos_graph);
    vector<vector<pair<int, int>>> graph = loadGraph("graph.txt", numNodes);

    int source = 0; 

    double start, end;

    if (algorithm == "B") {
        cout << "Running Bellman-Ford Algorithm Sequential\n";
        start = omp_get_wtime();
        vector<int> b_dist1 = bellmanFord(numNodes, graph, source);
		end = omp_get_wtime();

        cout<<"Time Taken:"<<(end-start)<<"\n";

        cout << "Running Bellman-Ford Algorithm Parallel\n";
        start = omp_get_wtime();
        vector<int> b_dist2 = parallelBellmanFord(numNodes, graph, source, numThreads);
        end = omp_get_wtime();

        checkEquality(b_dist1, b_dist2);

        cout<<"Time Taken:"<<(end-start)<<"\n";
    } 
    else if (algorithm == "F") {
        cout << "Running Floyd-Warshall Algorithm Sequential\n";
        vector<vector<int>> adjMatrix = convertToMatrix(graph, numNodes); // Convert graph to matrix format
        start = omp_get_wtime();
        vector<vector<int>> f_dist1 = floydWarshall(adjMatrix);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
        cout << "Running Floyd-Warshall Algorithm Parallel\n";
        start = omp_get_wtime();
        vector<vector<int>> f_dist2 = parallelFloydWarshall(adjMatrix, numThreads);
        end = omp_get_wtime();
        check2dEquality(f_dist1,f_dist2);
        cout<<"Time Taken:"<<(end-start)<<"\n";
    } 
    else if (algorithm == "J") {
        cout << "Running Johnsons Algorithm Sequential\n";
        vector<tuple<int, int, int>> edges = convertToEdgeList(graph);
        start = omp_get_wtime();
        vector<vector<int>> j_dist1 = johnsons(numNodes, edges);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
        cout << "Running Johnsons Algorithm Parallel\n";
        start = omp_get_wtime();
        vector<vector<int>> j_dist2 = johnsonsParallel(numNodes, edges, numThreads);
        end = omp_get_wtime();
        check2dEquality(j_dist1, j_dist2);
        cout<<"Time Taken:"<<(end-start)<<"\n";
    }
    else if (algorithm == "D") {
        cout << "Running Dijkstra Algorithm Sequential\n";
        start = omp_get_wtime();
        vector<int> d_dist1 = dijkstra(numNodes, graph, source);
        end = omp_get_wtime();
        cout << "Time Taken:" << (end - start) << "\n";

        cout << "Running Dijkstra Algorithm Parallel\n";
        start = omp_get_wtime();
        vector<int> d_dist2 = parallelDijkstra(numNodes, graph, source, numThreads);
        end = omp_get_wtime();
        checkEquality(d_dist1, d_dist2);
        cout << "Time Taken:" << (end - start) << "\n";
    }


    return 0;
}
