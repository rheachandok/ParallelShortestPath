#include "graph_utils.h"

void generateGraph(int nodes, const string &filename) {
    srand(time(0)); // Seed for random numbers
    int maxEdges = nodes * (nodes - 1); // Max edges in a directed graph (without self-loops)
    int numEdges = rand() % (maxEdges / 2) + nodes; // Ensure at least 'nodes' edges for connectivity

    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    outFile << nodes << "\n";
    for (int i = 0; i < numEdges; i++) {
        int u = rand() % nodes;  
        int v = rand() % nodes;  
        int weight = rand() % 100 - 50; 

        if (u != v) { 
            outFile << u << " " << v << " " << weight << "\n";
        }
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
