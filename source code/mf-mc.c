#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define V 6

bool bfs(int** residualGraph, int source, int sink, int* parent) {
    int visited[V] = {false};
    int queue[V];
    int front = 0, rear = 0;
    queue[rear++] = source;
    visited[source] = true;
    parent[source] = -1;

    while (front < rear) {
        int u = queue[front++];

        for (int v = 0; v < V; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                queue[rear++] = v;
                visited[v] = true;
                parent[v] = u;
                if (v == sink)
                    return true;
            }
        }
    }

    return false;
}

int edmondsKarp(int** graph, int source, int sink) {
    int** residualGraph = malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i) {
        residualGraph[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; ++j) {
            residualGraph[i][j] = graph[i][j];
        }
    }
    int parent[V];
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = 2147483647;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = (pathFlow < residualGraph[u][v])? (pathFlow) :residualGraph[u][v];
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

void dfs(int** residualGraph, int u, bool* visited) {
    visited[u] = true;
    for (int v = 0; v < V; ++v) {
        if (residualGraph[u][v] > 0 && !visited[v]) {
            dfs(residualGraph, v, visited);
        }
    }
}

void minCut(int** graph, int source, int sink, FILE* outputFile) {
    int** residualGraph = malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i) {
        residualGraph[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; ++j) {
            residualGraph[i][j] = graph[i][j];
        }
    }
    int maxFlow = edmondsKarp(graph, source, sink);
    bool visited[V];

    dfs(residualGraph, source, visited);

    fprintf(outputFile, "Flow Network Graphical Representation:\n");
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] > 0) {
                fprintf(outputFile, "%d -> %d (Capacity: %d)\n", u, v, graph[u][v]);
            }
        }
    }
    fprintf(outputFile, "\n");

    int t = 0;
    fprintf(outputFile, "Vertices on the source side of the minimum cut: \n");
    for (int u = 0; u < V; ++u) {
        if (visited[u]) {
            fprintf(outputFile, "%d\n", u);
            t++;
        }
    }
    fprintf(outputFile, "Edges in the Minimum Cut:%d\n", t);
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (visited[u] && !visited[v] && graph[u][v] > 0) {
                fprintf(outputFile, "%d -> %d (Capacity: %d)\n", u, v, graph[u][v]);
            }
        }
    }
    fprintf(outputFile, "\n");
    fprintf(outputFile, "Maximum Flow: %d\n", maxFlow);
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    FILE* outputFile = fopen("output.txt", "w");
    int** graph = malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i) {
        graph[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; ++j) {
            fscanf(inputFile, "%d", &graph[i][j]);
        }
    }
    int source, sink;
    fscanf(inputFile, "%d %d", &source, &sink);
    minCut(graph, source, sink, outputFile);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}