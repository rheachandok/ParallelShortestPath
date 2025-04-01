#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "graph_utils.h"
#include "algorithm_fns.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " B [s/p]" << endl;
        return 1;
    }

    string mode = argv[2];
    int nodes;
    vector<vector<pair<int, int>>> graph = loadGraph("graph.txt", nodes);

    int source = 0; // Example source node

    if (mode == "s") {
        bellmanFord(nodes, graph, source);
    } else if (mode == "p") {
        parallelBellmanFord(nodes, graph, source);
    } else {
        cerr << "Invalid mode! Use 's' for sequential or 'p' for parallel." << endl;
        return 1;
    }

    return 0;
}