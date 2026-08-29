// POJ 2522 - Partitioning for fun and profit
// Model: claude-sonnet-5
//
// A partition of m into n non-decreasing positive parts a1<=...<=an is
// found in lexicographic rank k by choosing a1 smallest-first: for a
// candidate value v for the current (smallest remaining) part, the number
// of ways to complete the remaining r-1 parts (each >= v, non-decreasing,
// summing to S-v) equals p(S - r*v, r-1), where p(x,j) counts partitions
// of x into AT MOST j parts (standard recurrence p(x,j)=p(x,j-1)+p(x-j,j),
// derived by conjugation with "parts of size <= j"). We scan v upward,
// subtracting counts from k until the block containing k is found, fix
// that part, and recurse on the remaining (r-1, S-v, lower bound v).
//
// Bound check: m<=220 (plain 220 in the HTML, not a mangled exponent) and
// n<=10, so the maximum count of partitions of m into n parts is under
// 2.6e9 (computed directly), well inside long long (and even unsigned
// 32-bit, though long long is used throughout for headroom). Discuss
// board confirms k fits comfortably (posters report even plain int
// working) and flags the real trap: forgetting the non-decreasing
// constraint a1<=...<=an, which this greedy-by-smallest-part construction
// enforces directly.
#include <cstdio>

static const int MAXM = 220;
static const int MAXN = 10;
static long long p[MAXM + 1][MAXN + 1]; // p[x][j] = partitions of x into at most j parts

int main() {
    for (int j = 0; j <= MAXN; ++j) p[0][j] = 1;
    for (int x = 1; x <= MAXM; ++x) {
        p[x][0] = 0;
        for (int j = 1; j <= MAXN; ++j) {
            p[x][j] = p[x][j - 1];
            if (x - j >= 0) p[x][j] += p[x - j][j];
        }
    }

    int c;
    if (scanf("%d", &c) != 1) return 0;
    while (c--) {
        int m, n;
        long long k;
        scanf("%d %d %lld", &m, &n, &k);

        int S = m, r = n, lo = 1;
        while (r > 0) {
            // find smallest v >= lo such that p(S - r*v, r-1) >= k
            for (int v = lo; ; ++v) {
                int rem = S - r * v;
                long long cnt = (rem < 0) ? 0 : p[rem][r - 1];
                if (k <= cnt) {
                    printf("%d\n", v);
                    S -= v;
                    r -= 1;
                    lo = v;
                    break;
                } else {
                    k -= cnt;
                }
            }
        }
    }
    return 0;
}
