
#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
using ll = long long;
const double PI = acos(-1.0);

// Recursive FFT using even and odd coefficients
vector<cd> fft(vector<cd> a) {
    int n = a.size();

    if (n == 1) {
        return a;
    }

    vector<cd> ae(n / 2), ao(n / 2);

    for (int i = 0; i < n / 2; i++) {
        ae[i] = a[2 * i];
        ao[i] = a[2 * i + 1];
    }

    vector<cd> ye = fft(ae);
    vector<cd> yo = fft(ao);

    vector<cd> y(n);
    cd w(1), wn(cos(2 * PI / n), sin(2 * PI / n));

    for (int k = 0; k < n / 2; k++) {
        y[k] = ye[k] + w * yo[k];
        y[k + n / 2] = ye[k] - w * yo[k];
        w *= wn;
    }

    return y;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<ll> a(n), b(m);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }

    // Pad to the smallest power of two >= n + m - 1
    int L = 1;
    while (L < n + m - 1) {
        L *= 2;
    }

    vector<cd> A(L), B(L);

    for (int i = 0; i < n; i++) {
        A[i] = a[i];
    }

    for (int i = 0; i < m; i++) {
        B[i] = b[i];
    }

    A = fft(A);
    B = fft(B);

    // Multiply corresponding FFT values
    for (int i = 0; i < L; i++) {
        A[i] *= B[i];
    }

    // Inverse FFT using conjugation
    for (cd& x : A) {
        x = conj(x);
    }

    A = fft(A);

    for (cd& x : A) {
        x = conj(x) / (double)L;
    }

    // Print the required integer coefficients
    for (int i = 0; i < n + m - 1; i++) {
        if (i > 0) {
            cout << ' ';
        }
        cout << llround(A[i].real());
    }

    cout << '\n';
    return 0;
}
