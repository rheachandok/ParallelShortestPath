#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "graph_utils.h"
#include "algorithm_fns.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " B/F <num_nodes> <num_threads>" << endl;
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

    if (algorithm == "B") {
        cout << "Running Bellman-Ford Algorithm...\n";
        bellmanFord(numNodes, graph, source);
        parallelBellmanFord(numNodes, graph, source, numThreads);
    } 
    else if (algorithm == "F") {
        cout << "Running Floyd-Warshall Algorithm...\n";
        vector<vector<int>> adjMatrix = convertToMatrix(graph, numNodes); // Convert graph to matrix format
        floydWarshall(adjMatrix);
        parallelFloydWarshall(adjMatrix, numThreads);
    } 
    else {
        cerr << "Invalid algorithm! Use 'B' for Bellman-Ford or 'F' for Floyd-Warshall." << endl;
        return 1;
    }

    return 0;
}
