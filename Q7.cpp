#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return false;
        }

        if (sz[a] < sz[b]) {
            swap(a, b);
        }

        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    // Store each road as {weight, u, v, index}
    vector<tuple<ll, int, int, int>> edges(M);

    for (int i = 0; i < M; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;

        edges[i] = {w, u, v, i + 1};
    }

    vector<bool> mandatory(M + 1, false);

    for (int i = 0; i < K; i++) {
        int idx;
        cin >> idx;
        mandatory[idx] = true;
    }

    DSU dsu(N);
    ll total = 0;
    int used = 0;

    // Include mandatory roads first
    for (auto [w, u, v, idx] : edges) {
        if (mandatory[idx]) {
            if (!dsu.unite(u, v)) {
                cout << -1 << '\n';
                return 0;
            }

            total += w;
            used++;
        }
    }

    // Sort roads by weight
    sort(edges.begin(), edges.end());

    // Add cheapest roads that do not create cycles
    for (auto [w, u, v, idx] : edges) {
        if (!mandatory[idx] && dsu.unite(u, v)) {
            total += w;
            used++;

            if (used == N - 1) {
                break;
            }
        }
    }

    if (used == N - 1) {
        cout << total << '\n';
    } else {
        cout << -1 << '\n';
    }

    return 0;
}
