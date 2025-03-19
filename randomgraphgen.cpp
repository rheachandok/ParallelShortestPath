#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to generate a random directed weighted graph
vector<vector<pair<int, int>>> generateGraph(int nodes) {
    vector<vector<pair<int, int>>> adjList(nodes);
    srand(time(0)); // Seed for random numbers

    int maxEdges = nodes * (nodes - 1); // Max edges in a directed graph
    int numEdges = rand() % (maxEdges / 2) + nodes; // Ensure at least 'nodes' edges for connectivity

    for (int i = 0; i < numEdges; i++) {
        int u = rand() % nodes;  // Random source node
        int v = rand() % nodes;  // Random destination node
        int weight = rand() % 100 + 1; // Random weight (1-100)

        if (u != v) { // Avoid self-loops
            adjList[u].push_back({v, weight});
        }
    }

    return adjList;
}

//Print graph
void printGraph(const vector<vector<pair<int, int>>> &adjList) {
    for (int i = 0; i < adjList.size(); i++) {
        cout << "Node " << i << " -> ";
        for (const auto &edge : adjList[i]) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << endl;
    }
}
