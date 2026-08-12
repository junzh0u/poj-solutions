// POJ 3597 - Polygon Division
// Model: gpt-5.6-terra
// Root a dissection at a fixed side.  The face containing that side is a
// triangle or quadrangle, so F = 1 + x F^2 + x^2 F^3.  Coefficients are
// maintained through the two product series; unsigned arithmetic is mod 2^64.
#include <iostream>

using namespace std;

int main() {
    const int MAXN = 4998;
    unsigned long long a[MAXN + 1], sq[MAXN + 1], cube[MAXN + 1];
    int n, i;
    a[0] = 1;
    sq[0] = 1;
    cube[0] = 1;
    for (n = 1; n <= MAXN; ++n) {
        a[n] = sq[n - 1];
        if (n >= 2) a[n] += cube[n - 2];
        sq[n] = 0;
        for (i = 0; i <= n; ++i) {
            sq[n] += a[i] * a[n - i];
        }
        cube[n] = 0;
        for (i = 0; i <= n; ++i) {
            cube[n] += a[i] * sq[n - i];
        }
    }
    while (cin >> n) cout << a[n - 2] << '\n';
    return 0;
}
