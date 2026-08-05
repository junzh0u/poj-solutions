// POJ 3071 - Football
//
// Single-elimination bracket over 2^n teams (the statement's "2n" is 2^n --
// the exponent is lost in the plain-text rendering; n <= 7, so at most 128
// teams).  p[i][j] is the probability i beats j.  Report the team most likely
// to win the whole tournament.
//
// Probability DP over rounds.  f[r][i] = P(team i is still alive after r
// rounds).  f[0][i] = 1.  To survive round r a team must both have reached it
// and beat whoever it meets there, and its round-r opponent is any team from
// the sibling half of its size-2^r block:
//
//     f[r][i] = f[r-1][i] * sum_{j in sibling block} f[r-1][j] * p[i][j]
//
// The sibling block is pure bit arithmetic: after r-1 rounds team i sits in
// block i >> (r-1), and the block it faces is that index with its low bit
// flipped, i.e. teams [s << (r-1), (s+1) << (r-1)) for s = (i >> (r-1)) ^ 1.
// Summing f[r-1][j]*p[i][j] over that block is exactly "beat whichever of them
// gets there", since the events are disjoint and the two halves are
// independent.
//
// O(n * 2^n * 2^(n-1)) per case -- trivial next to reading the matrix.
// Ties cannot decide the answer: the statement guarantees the top two differ
// by at least 0.01.  Input is a stream of cases terminated by a line "-1".

#include <cstdio>

static double p[128][128];
static double f[8][128];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != -1) {
        int m = 1 << n;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                scanf("%lf", &p[i][j]);
        for (int i = 0; i < m; i++) f[0][i] = 1.0;
        for (int r = 1; r <= n; r++) {
            for (int i = 0; i < m; i++) {
                double s = 0.0;
                int sib = (i >> (r - 1)) ^ 1;
                int lo = sib << (r - 1);
                int hi = lo + (1 << (r - 1));
                for (int j = lo; j < hi; j++) s += f[r - 1][j] * p[i][j];
                f[r][i] = f[r - 1][i] * s;
            }
        }
        int best = 0;
        for (int i = 1; i < m; i++)
            if (f[n][i] > f[n][best]) best = i;
        printf("%d\n", best + 1);
    }
    return 0;
}
