
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        iota(parent.begin(), parent.end(), 0);
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<tuple<ll, int, int>> edges;

    // Virtual city 0: edge cost represents building a station
    for (int i = 1; i <= N; i++) {
        ll cost;
        cin >> cost;
        edges.push_back({cost, 0, i});
    }

    // Original undirected power lines
    for (int i = 0; i < M; i++) {
        int u, v;
        ll cost;
        cin >> u >> v >> cost;
        edges.push_back({cost, u, v});
    }

    // Kruskal processes edges in increasing cost order
    sort(edges.begin(), edges.end());

    DSU dsu(N + 1);
    ll total = 0;
    int used = 0;

    for (auto [cost, u, v] : edges) {
        if (dsu.unite(u, v)) {
            total += cost;
            used++;

            if (used == N)
                break;
        }
    }

    cout << total << '\n';

    return 0;
}
