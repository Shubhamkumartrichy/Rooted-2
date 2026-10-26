#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    string S, P;
    cin >> S >> P;

    // Build the LPS array for the pattern
    vector<int> lps(M, 0);

    for (int i = 1, len = 0; i < M; ) {
        if (P[i] == P[len]) {
            lps[i++] = ++len;
        } else if (len > 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }

    vector<int> matches;
    int j = 0;

    // Scan N + M - 1 circular characters without creating S + S
    for (int i = 0; i < N + M - 1; i++) {
        char c = S[i % N];

        while (j > 0 && c != P[j]) {
            j = lps[j - 1];
        }

        if (c == P[j]) {
            j++;
        }

        if (j == M) {
            int start = i - M + 1;

            if (start < N) {
                matches.push_back(start);
            }

            j = lps[j - 1];
        }
    }

    cout << matches.size() << '\n';

    if (!matches.empty()) {
        for (int i = 0; i < (int)matches.size(); i++) {
            if (i > 0) {
                cout << ' ';
            }
            cout << matches[i];
        }
        cout << '\n';
    }

    return 0;
}
