// POJ 3186 - Treats for the Cows
//
// N treats in a row; each day exactly one is taken from the left or the right
// end, and a treat of value v sold on day a fetches v*a.  Maximise the total.
//
// Because one treat leaves per day, the age at which the next treat is sold is
// determined entirely by how many are left: a remaining run of length `len` is
// sold starting at age n-len+1.  So the age is a *function of the interval*,
// not extra state, and the interval DP is two-dimensional only:
//
//   dp[i][j] = max(age*v[i] + dp[i+1][j],          // take from the left end
//                  age*v[j] + dp[i][j-1])          // take from the right end
//   with age = n - (j-i+1) + 1, and dp over a single treat = v[i]*n.
//
// Filled by increasing run length, dp[i][j] depends only on intervals one
// shorter, so a 1-D rolling array f[i] = dp[i][i+len-1] suffices: f[i]'s new
// value reads f[i] and f[i+1] of the previous length, both at indices >= i, so
// an in-place forward sweep is safe.  O(n^2) time, O(n) memory — 15MS/376K
// against limits of 1000MS/65536K, where the full 2000x2000 table would have
// been 16MB of int (or 32MB of long long).
//
// Statement notes: "age" is the number of days elapsed including the day of
// sale, so the very first treat sold has a=1, not a=0 — the sample's
// 1x1 + 2x2 + 3x3 + 4x1 + 5x5 = 43 pins this down.  The maximum possible
// answer, 1000 * (1+2+...+2000) = 2,001,000,000, only just fits in a signed
// 32-bit int, so the accumulators are long long; output goes through
// std::cout rather than printf to sidestep POJ's %lld/%I64d compiler quirk.

#include <cstdio>
#include <iostream>

static int v[2005];
static long long f[2005];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) scanf("%d", &v[i]);

    /* f[i] holds dp for the interval starting at i of the current length.
       Interval of length len is sold starting at age n-len+1. */
    for (int i = 0; i < n; ++i) f[i] = (long long)v[i] * n;

    for (int len = 2; len <= n; ++len) {
        long long age = n - len + 1;
        for (int i = 0; i + len - 1 < n; ++i) {
            long long takeLeft = age * v[i] + f[i + 1];
            long long takeRight = age * v[i + len - 1] + f[i];
            f[i] = takeLeft > takeRight ? takeLeft : takeRight;
        }
    }
    std::cout << f[0] << std::endl;
    return 0;
}
