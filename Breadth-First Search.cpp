#include <bits/stdc++.h>
using namespace std;

vector<int> bfsOfGraph(int V, vector<int> adj[]) {
    vector<int> vis(V, 0);
    queue<int> q;
    vector<int> bfs;

    vis[0] = 1;
    q.push(0);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        bfs.push_back(node);

        for (int it : adj[node]) {
            if (!vis[it]) {
                vis[it] = 1;
                q.push(it);
            }
        }
    }

    return bfs;
}

int main() {
    int V, E;
    cin >> V >> E;

    vector<int> adj[V];

    // Input edges of the undirected graph.
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> ans = bfsOfGraph(V, adj);

    for (int node : ans) {
        cout << node << " ";
    }
    cout << '\n';

    return 0;
}