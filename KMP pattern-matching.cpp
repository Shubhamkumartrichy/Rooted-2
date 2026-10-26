
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S, P;
    cin >> S >> P;

    int N = S.size(), M = P.size();
    vector<int> lps(M, 0);

    // Build the LPS array
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

    // Search for the pattern in the text
    for (int i = 0; i < N; i++) {
        while (j > 0 && S[i] != P[j])
            j = lps[j - 1];

        if (S[i] == P[j])
            j++;

        if (j == M) {
            matches.push_back(i - M + 1);
            j = lps[j - 1];
        }
    }

    cout << matches.size() << '\n';

    if (!matches.empty()) {
        for (int i = 0; i < (int)matches.size(); i++) {
            if (i > 0) cout << ' ';
            cout << matches[i];
        }
        cout << '\n';
    }

    return 0;
}
