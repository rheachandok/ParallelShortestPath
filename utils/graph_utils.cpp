#include "graph_utils.h"

#include <vector>
#include <tuple>
#include <set>

using namespace std;

vector<tuple<int, int, int>> convertToEdgeList(const vector<vector<pair<int, int>>> &graph) {
    vector<tuple<int, int, int>> edgeList;

    for (int u = 0; u < graph.size(); u++) {
        for (const auto &edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            edgeList.push_back(make_tuple(u, v, weight));
        }
    }

    return edgeList;
}

void generateGraph(int nodes, const string &filename, bool nonNegativeWeights = false, int sparsity_factor = 2) {
    srand(time(0));
    int maxPossibleEdges = nodes * (nodes - 1);
    
    // Calculate number of edges based on sparsity factor
    int maxAllowedEdges = maxPossibleEdges / max(1, sparsity_factor);
    int minEdges = 1; // Minimum 1 edge to ensure some connectivity
    int numEdges = minEdges + (maxAllowedEdges > minEdges ? rand() % (maxAllowedEdges - minEdges) : 0);

    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    outFile << nodes << "\n";

    set<pair<int, int>> edgeSet;

    while (edgeSet.size() < numEdges) {
        int u = rand() % nodes;
        int v = rand() % nodes;

        if (u == v) continue;
        if (edgeSet.count({u, v})) continue;

        edgeSet.insert({u, v});

        int weight;
        if (nonNegativeWeights)
            weight = rand() % 50 + 1;  // 1-50 positive weights
        else
            weight = (rand() % 100) - 50;  // -50 to 49 weights

        outFile << u << " " << v << " " << weight << "\n";
    }

    outFile.close();
}


void printGraph(const vector<vector<pair<int, int>>> &adjList) {
    for (int i = 0; i < adjList.size(); i++) {
        cout << "Node " << i << " -> ";
        for (const auto &edge : adjList[i]) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }
}

vector<vector<pair<int, int>>> loadGraph(const string &filename, int &nodes) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    inFile >> nodes;
    vector<vector<pair<int, int>>> adjList(nodes);
    int u, v, weight;
    while (inFile >> u >> v >> weight) {
        adjList[u].push_back({v, weight});
    }
    inFile.close();
    return adjList;
}

vector<vector<int>> convertToMatrix(const vector<vector<pair<int, int>>> &graph, int numNodes) {
    // Initialize adjacency matrix with INF (no direct edge)
    vector<vector<int>> matrix(numNodes, vector<int>(numNodes, numeric_limits<int>::max()));

    // Set diagonal to 0 (distance from node to itself)
    for (int i = 0; i < numNodes; i++) {
        matrix[i][i] = 0;
    }

    // Fill in the adjacency matrix from the adjacency list
    for (int u = 0; u < numNodes; u++) {
        for (const auto &edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            matrix[u][v] = weight;
        }
    }

    return matrix;
}
