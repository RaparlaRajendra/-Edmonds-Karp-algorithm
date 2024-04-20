from collections import deque

def bfs(residual_graph, source, sink, parent):
    visited = [False] * len(residual_graph)
    queue = deque([source])
    visited[source] = True
    parent[source] = -1

    while queue:
        u = queue.popleft()

        for v in range(len(residual_graph)):
            if not visited[v] and residual_graph[u][v] > 0:
                queue.append(v)
                visited[v] = True
                parent[v] = u
                if v == sink:
                    return True

    return False

def edmonds_karp(graph, source, sink):
    n = len(graph)
    residual_graph = [[0] * n for _ in range(n)]
    
    for i in range(n):
        for j in range(n):
            residual_graph[i][j] = graph[i][j]

    parent = [-1] * n
    max_flow = 0

    while bfs(residual_graph, source, sink, parent):
        path_flow = float('inf')
        v = sink

        while v != source:
            u = parent[v]
            path_flow = min(path_flow, residual_graph[u][v])
            v = u

        v = sink
        while v != source:
            u = parent[v]
            residual_graph[u][v] -= path_flow
            residual_graph[v][u] += path_flow
            v = u

        max_flow += path_flow

    return max_flow, residual_graph

def dfs(graph, u, visited):
    visited[u] = True
    for v in range(len(graph)):
        if graph[u][v] > 0 and not visited[v]:
            dfs(graph, v, visited)

def min_cut(graph, source, sink, input_file, output_file):
    max_flow, residual_graph = edmonds_karp(graph, source, sink)

    output_file.write("Flow Network Graphical Representation:\n")
    for i in range(len(graph)):
        for j in range(len(graph)):
            if graph[i][j] > 0:
                output_file.write(f"{i} -> {j} (Capacity: {graph[i][j]})\n")
    output_file.write("\n")

    visited = [False] * len(graph)
    dfs(graph, source, visited)
    output_file.write("Vertices on the source side of the minimum cut:\n")
    for i in range(len(visited)):
        if visited[i]:
            output_file.write(f"{i}\n")

    for i in range(len(graph)):
        for j in range(len(graph)):
            if visited[i] and not visited[j] and graph[i][j] > 0:
                output_file.write(f"{i} -> {j} (Capacity: {graph[i][j]})\n")

    output_file.write("\n")
    output_file.write(f"Edges in the Minimum Cut: {sum(visited)}\n")
    output_file.write(f"Maximum Flow: {max_flow}\n")

def main():
    input_file = open("input.txt", "r")
    output_file = open("output.txt", "w")

    source, sink = map(int, input_file.readline().split())
    graph = [list(map(int, input_file.readline().split())) for _ in range(6)]

    min_cut(graph, source, sink, input_file, output_file)

    input_file.close()
    output_file.close()

if __name__ == "__main__":
    main()
