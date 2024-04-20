#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

#define V 6 

bool bfs(vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    vector<bool> visited(V, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                visited[v] = true;
                parent[v] = u;
                if (v == sink)
                    return true;
            }
        }
    }

    return false;
}

int edmondsKarp(vector<vector<int>>& graph, int source, int sink) {
    vector<vector<int>> residualGraph = graph; 
    vector<int> parent(V, -1);
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

void dfs(vector<vector<int>>& residualGraph, int u, vector<bool>& visited) {
    visited[u] = true;
    for (int v = 0; v < V; ++v) {
        if (residualGraph[u][v] > 0 && !visited[v]) {
            dfs(residualGraph, v, visited);
        }
    }
}

void minCut(vector<vector<int>>& graph, int source, int sink, ofstream& outputFile) {
    vector<vector<int>> residualGraph = graph;
    int maxFlow = edmondsKarp(graph, source, sink);
    vector<bool> visited(V, false);

    dfs(residualGraph, source, visited);

    outputFile << "Flow Network Graphical Representation:" << endl;
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] > 0) {
                outputFile << u << " -> " << v << " (Capacity: " << graph[u][v] << ")" << endl;
            }
        }
    }
    outputFile << endl;

    int t=0;
    outputFile << "Vertices on the source side of the minimum cut: " << endl;
    for (int u = 0; u < V; ++u) {
        if (visited[u]) {
            outputFile << u << endl;
            t++;
        }
    }
    outputFile << "Edges in the Minimum Cut:" << t<< endl;
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
                if (visited[u] && !visited[v] && graph[u][v] > 0) {
                outputFile << u << " -> " << v << " (Capacity: " << graph[u][v] << ")" << endl;
            }
        }
    }
    outputFile << endl;
    outputFile << "Maximum Flow: " << maxFlow << endl;
}

int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");
    vector<vector<int>> graph(V, vector<int>(V, 0));
    int source, sink;
    inputFile >> source >> sink;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            inputFile >> graph[i][j];
        }
    }
    minCut(graph, source, sink, outputFile);
    inputFile.close();
    outputFile.close();

    return 0;
}
