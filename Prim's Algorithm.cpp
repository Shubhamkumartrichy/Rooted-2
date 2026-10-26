#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll prims(int V, vector<pair<int, ll>> adj[]) {
    vector<int> vis(V, 0);

    // {weight, node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    pq.push({0, 0});
    ll mstWeight = 0;
    int count = 0;

    while (!pq.empty()) {
        auto [wt, node] = pq.top();
        pq.pop();

        if (vis[node])
            continue;

        vis[node] = 1;
        mstWeight += wt;
        count++;

        for (auto [adjNode, edgeWt] : adj[node]) {
            if (!vis[adjNode]) {
                pq.push({edgeWt, adjNode});
            }
        }
    }

    // A disconnected graph has no spanning tree.
    if (count != V)
        return -1;

    return mstWeight;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;

    vector<pair<int, ll>> adj[V];

    // Input format: u v weight (undirected graph).
    for (int i = 0; i < E; i++) {
        int u, v;
        ll wt;
        cin >> u >> v >> wt;

        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt});
    }

    ll ans = prims(V, adj);

    if (ans == -1)
        cout << "MST does not exist\n";
    else
        cout << "MST weight: " << ans << '\n';

    return 0;
}