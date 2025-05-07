#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <unordered_set>
using namespace std;

// Graph represented as adjacency list
class Graph {
    int V; // number of vertices
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        // adj[v].push_back(u); // Uncomment for undirected graph
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        vector<int> curr_level;
        curr_level.push_back(start);
        visited[start] = true;

        cout << "BFS traversal: " << start << " ";

        while (!curr_level.empty()) {
            vector<int> next_level;

            // Parallel processing of current level
            #pragma omp parallel
            {
                vector<int> local_next;

                #pragma omp for nowait
                for (int i = 0; i < curr_level.size(); i++) {
                    int u = curr_level[i];
                    for (int v : adj[u]) {
                        // Avoid race condition using atomic or critical
                        #pragma omp critical
                        {
                            if (!visited[v]) {
                                visited[v] = true;
                                local_next.push_back(v);
                            }
                        }
                    }
                }

                // Merge local_next into next_level
                #pragma omp critical
                next_level.insert(next_level.end(), local_next.begin(), local_next.end());
            }

            for (int v : next_level)
                cout << v << " ";

            curr_level = next_level;
        }

        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    Graph g(V);
    cout << "Enter edges (u v):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter start vertex: ";
    cin >> start;

    double start_time = omp_get_wtime();
    g.parallelBFS(start);
    double end_time = omp_get_wtime();

    cout << "Time taken: " << (end_time - start_time) << " seconds." << endl;

    return 0;
}
