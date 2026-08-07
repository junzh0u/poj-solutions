// POJ 2193 - Lenny's Lucky Lotto Lists
// Model: claude-sonnet-5
// Approach: For each length i (1..10) and threshold value v (0..2000), compute
// f[i][v] = number of valid lucky lists of length i whose last (largest)
// chosen number equals exactly v, and F[i][v] = cumulative sum of f[i][u]
// for u=1..v (i.e. the count of length-i lists whose max is <= v). The base
// case is f[1][v] = 1 for every v>=1 (a single number has no extra
// constraint), so F[1][v] = v. The recurrence f[i][v] = F[i-1][v/2] holds
// because once v is fixed as the last number, the remaining (i-1)-length
// prefix must be a valid lucky list whose own maximum is <= floor(v/2), to
// satisfy "each number is at least twice as large as the one before it";
// F[i][*] is then just the prefix sum of f[i][*]. The table is built once
// for the whole domain (N<=10, M<=2000) and every test case is answered in
// O(1) as F[n][m].
// Values fit comfortably in a 64-bit integer (max ~5.7e12 at N=10, M=2000),
// so plain `long long` with iostream output sidesteps the %lld vs %I64d
// format-string dispute the discuss board raises between POJ's C++ and G++
// compilers (one board post reports long long + G++ giving WA where __int64
// + %I64d under C++ was Accepted, which reads as a printf format-string
// mismatch rather than a real overflow, since iostream has no such
// specifier to get wrong).
// No statement ambiguity beyond the standard "at least twice as large"
// reading; verified against all three sample cases and a brute-force
// differential test enumerating every lucky list for n=1..5, m<=39.

#include <iostream>
using namespace std;

static long long f[11][2001];
static long long F[11][2001];

int main() {
    for (int v = 1; v <= 2000; ++v) f[1][v] = 1;
    for (int v = 0; v <= 2000; ++v) F[1][v] = v;
    for (int i = 2; i <= 10; ++i) {
        for (int v = 0; v <= 2000; ++v) {
            int half = v / 2;
            f[i][v] = (v >= 1) ? F[i - 1][half] : 0;
            F[i][v] = (v > 0 ? F[i][v - 1] : 0) + f[i][v];
        }
    }

    int c;
    cin >> c;
    for (int t = 1; t <= c; ++t) {
        int n, m;
        cin >> n >> m;
        cout << "Case " << t << ": n = " << n << ", m = " << m
             << ", # lists = " << F[n][m] << "\n";
    }
    return 0;
}
