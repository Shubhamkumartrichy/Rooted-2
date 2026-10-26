// i am using the idea of Kosaraju's algorithm

#include <bits/stdc++.h>
using namespace std;

// Convert a signed literal into a graph vertex
int literal(int x, int n) {
    if (x > 0) {
        return x - 1;
    }
    return n + (-x) - 1;
}

// Return the vertex representing the negation
int neg(int x, int n) {
    if (x < n) {
        return x + n;
    }
    return x - n;
}

// Add an edge to the graph and its reverse
void addEdge(int u, int v, vector<vector<int>>& g, vector<vector<int>>& rg) {
    g[u].push_back(v);
    rg[v].push_back(u);
}

// First DFS: calculate finishing order
void dfs1(int s, vector<vector<int>>& g, vector<bool>& vis, vector<int>& order) {
    stack<pair<int, int>> st;
    vis[s] = true;
    st.push({s, 0});

    while (!st.empty()) {
        int u = st.top().first;
        int& i = st.top().second;

        if (i < (int)g[u].size()) {
            int v = g[u][i++];
            if (!vis[v]) {
                vis[v] = true;
                st.push({v, 0});
            }
        } else {
            order.push_back(u);
            st.pop();
        }
    }
}

// Second DFS: assign strongly connected components
void dfs2(int s, vector<vector<int>>& rg, vector<int>& comp, int id) {
    stack<int> st;
    st.push(s);
    comp[s] = id;

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        for (int v : rg[u]) {
            if (comp[v] == -1) {
                comp[v] = id;
                st.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        int V = 2 * n;
        vector<vector<int>> g(V), rg(V);

        for (int i = 0; i < m; i++) {
            int a, b;
            cin >> a >> b;

            int A = literal(a, n);
            int B = literal(b, n);

            // (A OR B) becomes (!A -> B) and (!B -> A)
            addEdge(neg(A, n), B, g, rg);
            addEdge(neg(B, n), A, g, rg);
        }

        vector<bool> vis(V, false);
        vector<int> order;

        for (int i = 0; i < V; i++) {
            if (!vis[i]) {
                dfs1(i, g, vis, order);
            }
        }

        reverse(order.begin(), order.end());

        vector<int> comp(V, -1);
        int id = 0;

        for (int u : order) {
            if (comp[u] == -1) {
                dfs2(u, rg, comp, id++);
            }
        }

        vector<int> ans(n);
        bool ok = true;

        // A variable and its negation cannot share an SCC
        for (int i = 0; i < n; i++) {
            if (comp[i] == comp[i + n]) {
                ok = false;
                break;
            }

            ans[i] = (comp[i] > comp[i + n]) ? 1 : 0;
        }

        if (!ok) {
            cout << "Unsatisfiable\n";
        } else {
            for (int i = 0; i < n; i++) {
                if (i > 0) {
                    cout << ' ';
                }
                cout << ans[i];
            }
            cout << '\n';
        }
    }

    return 0;
}
