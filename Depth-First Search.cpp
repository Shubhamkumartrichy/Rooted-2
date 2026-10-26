#include <bits/stdc++.h>
using namespace std;

void dfs(int node, vector<int> adj[], vector<int>& vis, vector<int>& ans) {
    vis[node] = 1;
    ans.push_back(node);

    for (int it : adj[node]) {
        if (!vis[it]) {
            dfs(it, adj, vis, ans);
        }
    }
}

vector<int> dfsOfGraph(int V, vector<int> adj[]) {
    vector<int> vis(V, 0);
    vector<int> ans;

    dfs(0, adj, vis, ans);

    return ans;
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

    vector<int> ans = dfsOfGraph(V, adj);

    for (int node : ans) {
        cout << node << " ";
    }
    cout << '\n';

    return 0;
}