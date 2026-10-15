// POJ 1641 - Rational Approximation
// Model: gpt-6.1-sol
// Solve the last n convolution equations by exact Cramer's rule, then
// recover p from the first m coefficients. Missing negative f indices are 0.
// n <= 4 and |f| <= 5 keep all determinant arithmetic within 32-bit int.
#include <iostream>
#include <cstdlib>
using namespace std;
int determinant(int a[4][4], int n) {
    if (n == 1) return a[0][0];
    int result = 0;
    for (int c = 0; c < n; ++c) {
        int b[4][4];
        for (int i = 1; i < n; ++i) {
            int k = 0;
            for (int j = 0; j < n; ++j)
                if (j != c) b[i-1][k++] = a[i][j];
        }
        int term = a[0][c] * determinant(b, n-1);
        result += c % 2 ? -term : term;
    }
    return result;
}
int gcd(int a, int b) {
    while (b) { int r = a % b; a = b; b = r; }
    return a;
}
void print(int a[], int n, int denominator) {
    bool first = true;
    for (int i = 0; i < n; ++i) if (a[i]) {
        int numerator = a[i], d = denominator;
        if (d < 0) { d = -d; numerator = -numerator; }
        int g = gcd(abs(numerator), d);
        numerator /= g; d /= g;
        if (!first) cout << ' ';
        first = false;
        cout << '(' << numerator;
        if (d != 1) cout << '/' << d;
        cout << ',' << i << ')';
    }
    if (first) cout << "(0,0)";
    cout << '\n';
}
int main() {
    int m, n;
    bool first = true;
    while (cin >> m >> n && (m || n)) {
        int f[10], a[4][4], q[4], p[10] = {0};
        for (int i = 0; i < m+n; ++i) cin >> f[i];
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                a[i][j] = m+i-j < 0 ? 0 : f[m+i-j];
        int d = determinant(a, n);
        for (int c = 0; c < n; ++c) {
            int b[4][4];
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    b[i][j] = j == c ? (i == n-1 ? 1 : 0) : a[i][j];
            q[c] = determinant(b, n);
        }
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n && j <= i; ++j)
                p[i] += q[j] * f[i-j];
        if (!first) cout << '\n';
        first = false;
        print(p, m, d); print(q, n, d);
    }
    return 0;
}
