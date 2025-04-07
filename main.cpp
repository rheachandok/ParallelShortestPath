#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "graph_utils.h"
#include "algorithm_fns.h"
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " B/F/J <num_nodes> <num_threads>" << endl;
        return 1;
    }

    string algorithm = argv[1];
    int numNodes = atoi(argv[2]);
    int numThreads = atoi(argv[3]);

    // Generate and load graph
    generateGraph(numNodes, "graph.txt");
    vector<vector<pair<int, int>>> graph = loadGraph("graph.txt", numNodes);
    printGraph(graph);

    int source = 0; 

    double start, end;

    if (algorithm == "B") {
        cout << "Running Bellman-Ford Algorithm Sequential\n";
        start = omp_get_wtime();
        bellmanFord(numNodes, graph, source);
		end = omp_get_wtime();

        cout<<"Time Taken:"<<(end-start);

        cout << "Running Bellman-Ford Algorithm Parallel\n";
        start = omp_get_wtime();
        parallelBellmanFord(numNodes, graph, source, numThreads);
        end = omp_get_wtime();

        cout<<"Time Taken:"<<(end-start)<<"\n";
    } 
    else if (algorithm == "F") {
        cout << "Running Floyd-Warshall Algorithm Sequential\n";
        vector<vector<int>> adjMatrix = convertToMatrix(graph, numNodes); // Convert graph to matrix format
        start = omp_get_wtime();
        floydWarshall(adjMatrix);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
        cout << "Running Floyd-Warshall Algorithm Parallel\n";
        start = omp_get_wtime();
        parallelFloydWarshall(adjMatrix, numThreads);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
    } 
    else if (algorithm == "J") {
        cout << "Running Johnsons Algorithm Sequential\n";
        vector<tuple<int, int, int>> edges = convertToEdgeList(graph);
        start = omp_get_wtime();
        johnsons(numNodes, edges);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
        cout << "Running Johnsons Algorithm Parallel\n";
        start = omp_get_wtime();
        johnsonsParallel(numNodes, edges, numThreads);
        end = omp_get_wtime();
        cout<<"Time Taken:"<<(end-start)<<"\n";
    }

    return 0;
}
