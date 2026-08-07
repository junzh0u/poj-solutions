// POJ 3036 - Honeycomb Walk
// Model: claude-opus-5
//
// Count closed walks of length n (1 <= n <= 14) on the hexagonal lattice: a
// larva starts in a cell, makes n steps into adjacent cells, and must finish
// in the starting cell.
//
// Axial coordinates make the hex lattice a plain integer grid: a cell is
// (x, y) and its six neighbours are (+1,0) (-1,0) (0,+1) (0,-1) (+1,-1)
// (-1,+1).  (Equivalently, six unit plane vectors 60 degrees apart, of which
// only two are independent.)  So the answer is a straight DP over steps:
//   dp[k][cell] = sum over the six neighbours of dp[k-1][neighbour],
// with dp[0][origin] = 1, and the answer is dp[n][origin].  After n <= 14
// steps no cell further than 14 from the origin is reachable, so a
// 29 x 29 board with an offset of 14 is enough.
//
// The whole table for n = 1..14 is precomputed once (14 values, the entire
// input domain) and each query is a lookup.  Every intermediate count fits in
// a 32-bit signed int: the origin is the heaviest cell at every step, and its
// largest value over the whole run is dp[14][origin] = 1488801600 < 2^31 - 1.
//
// No statement ambiguity: "six adjacent cells" fixes the move set, and steps
// may revisit cells freely (nothing forbids it), which the sample confirms --
// n = 4 gives 90, which counts walks that backtrack.

#include <cstdio>
#include <cstring>

static const int OFF = 14;
static const int SZ = 2 * OFF + 1;

int main() {
    static const int dx[6] = {1, -1, 0, 0, 1, -1};
    static const int dy[6] = {0, 0, 1, -1, -1, 1};

    int ans[15];
    int cur[SZ][SZ], nxt[SZ][SZ];

    memset(cur, 0, sizeof(cur));
    cur[OFF][OFF] = 1;
    ans[0] = 1;
    for (int step = 1; step <= 14; ++step) {
        memset(nxt, 0, sizeof(nxt));
        for (int x = 0; x < SZ; ++x) {
            for (int y = 0; y < SZ; ++y) {
                int v = cur[x][y];
                if (!v) continue;
                for (int d = 0; d < 6; ++d) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (nx < 0 || nx >= SZ || ny < 0 || ny >= SZ) continue;
                    nxt[nx][ny] += v;
                }
            }
        }
        memcpy(cur, nxt, sizeof(cur));
        ans[step] = cur[OFF][OFF];
    }

    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n;
        if (scanf("%d", &n) != 1) break;
        if (n < 0) n = 0;
        if (n > 14) n = 14;
        printf("%d\n", ans[n]);
    }
    return 0;
}
