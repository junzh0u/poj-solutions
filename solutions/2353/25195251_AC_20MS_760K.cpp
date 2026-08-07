// POJ 2353 - Ministry
// Model: claude-opus-5
//
// Node-weighted shortest chain on an M x N grid: an official (l,k) can sign once
// (l-1,k) has signed (rule b) or a same-floor neighbour (l,k+-1) has signed
// (rule c), and every floor-1 official signs unconditionally (rule a).  So a
// cheapest approval is a chain that never moves downwards and, on each floor,
// walks a contiguous run of rooms before stepping up.
//
// DP floor by floor.  With dp[l-1][] known, let base[k] = dp[l-1][k] + fee[l][k]
// (arrive from below).  A left-to-right sweep L[k] = min(base[k], L[k-1]+fee)
// covers runs walked rightwards, a right-to-left sweep R[k] = min(base[k],
// R[k+1]+fee) covers runs walked leftwards, and dp[l][k] = min(L[k], R[k]).
// Floor 1 falls out of the same code with dp[0][] = 0.  O(M*N).
//
// Two traps, both reported on the discuss board:
//
// * Intermediate values overflow 32 bits.  The hint bounds each *optimal* cost
//   by 1e9, but base[k] alone reaches 2e9 and a sweep accumulating a whole floor
//   of 1e9 fees reaches 5e11, so the sweeps run in long long (never printed, so
//   POJ's %lld quirk is irrelevant).
//
// * Path reconstruction can cycle, which is the OLE the board complains about.
//   Following parents off the merged dp[] can hop k -> k-1 via the L-sweep and
//   then k-1 -> k via the R-sweep, looping forever.  So the reconstruction
//   remembers which sweep it is in and stays there, making k monotone within a
//   floor; ties prefer stepping up over walking sideways, which also keeps the
//   printed path short.
//
// Output is room numbers only, so the judge recovers floors from the sequence:
// a repeated number means "one floor up", a number differing by one means "same
// floor, neighbouring room".  The path is emitted bottom-up, ending on floor M.

#include <cstdio>

static const int MAXM = 105;
static const int MAXN = 505;

static int fee[MAXM][MAXN];
static long long prevRow[MAXN], L[MAXN], R[MAXN];
static unsigned char Lup[MAXM][MAXN], Rup[MAXM][MAXN], useR[MAXM][MAXN];
static int path[MAXM * MAXN + 10];

int main() {
    int m, n;
    if (scanf("%d %d", &m, &n) != 2) return 0;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            if (scanf("%d", &fee[i][j]) != 1) return 0;

    for (int j = 1; j <= n; ++j) prevRow[j] = 0;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            L[j] = prevRow[j] + fee[i][j];
            Lup[i][j] = 1;
            if (j > 1 && L[j - 1] + fee[i][j] < L[j]) {
                L[j] = L[j - 1] + fee[i][j];
                Lup[i][j] = 0;
            }
        }
        for (int j = n; j >= 1; --j) {
            R[j] = prevRow[j] + fee[i][j];
            Rup[i][j] = 1;
            if (j < n && R[j + 1] + fee[i][j] < R[j]) {
                R[j] = R[j + 1] + fee[i][j];
                Rup[i][j] = 0;
            }
        }
        for (int j = 1; j <= n; ++j) {
            if (R[j] < L[j]) { prevRow[j] = R[j]; useR[i][j] = 1; }
            else { prevRow[j] = L[j]; useR[i][j] = 0; }
        }
    }

    int best = 1;
    for (int j = 2; j <= n; ++j)
        if (prevRow[j] < prevRow[best]) best = j;

    int len = 0;
    int i = m, j = best;
    int mode = useR[m][best];
    while (i >= 1) {
        path[len++] = j;
        if (mode == 0) {
            if (Lup[i][j]) { --i; if (i >= 1) mode = useR[i][j]; }
            else --j;
        } else {
            if (Rup[i][j]) { --i; if (i >= 1) mode = useR[i][j]; }
            else ++j;
        }
    }

    for (int k = len - 1; k >= 0; --k) printf("%d\n", path[k]);
    return 0;
}
