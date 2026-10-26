
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (1LL << 62);

using Edge = pair<int, ll>;
using Graph = vector<vector<Edge>>;

// Dijkstra's algorithm, here i am applying it to just non-negative edges.
vector<ll> dijkstra(int source, const Graph& adj) {
    int n = adj.size();
    vector<ll> dist(n, INF);

    // Min-priority queue stores {distance, vertex}
    priority_queue<pair<ll, int>,vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u])
            continue;

        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, S, T;
    cin >> N >> M >> S >> T;

    // Cities are numbered from 1 to N
    Graph adj(N + 1), rev(N + 1);

    int negU = -1, negV = -1;
    ll negW = 0;

    for (int i = 0; i < M; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;

        if (w < 0) {
            // Store the negative edge separately
            negU = u;
            negV = v;
            negW = w;
        } else {
            adj[u].push_back({v, w});
            rev[v].push_back({u, w});
        }
    }

    // Shortest distances from S and distances from every city to T
    vector<ll> fromS = dijkstra(S, adj);
    vector<ll> toT = dijkstra(T, rev);

    // Best path without using the negative edge
    ll ans = fromS[T];

    // Final answer
    if (negU != -1 && fromS[negU] != INF && toT[negV] != INF) {
        ans = min(ans, fromS[negU] + negW + toT[negV]);
    }

    if (ans == INF){
        cout << "UNREACHABLE\n";
    }else{
        cout << ans << '\n';
    }

    return 0;
}
