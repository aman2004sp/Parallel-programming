#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
using namespace std;

#define INF INT_MAX

void dijkstraParallel(const vector<vector<int>>& graph, int src, vector<int>& dist) {
    int V = graph.size();
    vector<bool> visited(V, false);
    dist.assign(V, INF);
    dist[src] = 0;

    for (int count = 0; count < V - 1; ++count) {
        int u = -1;
        int min_dist = INF;

        // Parallel reduction to find the unvisited node with the smallest distance
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                #pragma omp critical
                {
                    if (dist[v] < min_dist) {
                        min_dist = dist[v];
                        u = v;
                    }
                }
            }
        }

        if (u == -1) break; // Remaining vertices are unreachable

        visited[u] = true;

        // Parallel update of neighbors
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                #pragma omp critical
                dist[v] = min(dist[v], dist[u] + graph[u][v]);
            }
        }
    }
}

int main() {
    int V;
    cout << "Enter number of vertices: ";
    cin >> V;

    vector<vector<int>> graph(V, vector<int>(V));
    cout << "Enter adjacency matrix (use 0 for no edge):\n";
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            cin >> graph[i][j];

    int src;
    cout << "Enter source vertex: ";
    cin >> src;

    vector<int> dist;
    double start_time = omp_get_wtime();
    dijkstraParallel(graph, src, dist);
    double end_time = omp_get_wtime();

    cout << "Shortest distances from source " << src << ":\n";
    for (int i = 0; i < V; i++)
        cout << "To " << i << ": " << (dist[i] == INF ? -1 : dist[i]) << endl;

    cout << "Time taken: " << (end_time - start_time) << " seconds.\n";

    return 0;
}
