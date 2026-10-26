
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (sz[a] < sz[b])
            swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

int main() {
    int V, E;
    cin >> V >> E;

    vector<tuple<ll, int, int>> edges;

    for (int i = 0; i < E; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    // Process edges in increasing order of weight
    sort(edges.begin(), edges.end());

    DSU dsu(V);
    ll total = 0;
    int count = 0;

    for (auto [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            total += w;
            count++;

            cout << u << " - " << v << " : " << w << '\n';

            if (count == V - 1)
                break;
        }
    }

    if (count == V - 1)
        cout << "MST cost: " << total << '\n';
    else
        cout << "Graph is disconnected; no MST exists.\n";

    return 0;
}
