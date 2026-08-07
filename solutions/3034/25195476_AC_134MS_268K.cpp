// POJ 3034 - Whac-a-Mole
// Model: claude-sonnet-5
// Approach: DP over hammer position on an extended integer grid.
// The hammer may stand on ANY integer point, including outside/negative of
// the [0,n) board (confirmed by the discuss board), so pad the coordinate
// range by d on every side: shifted coordinates live in [0, n+2d).  Any
// position further than d outside the original board can never again reach
// a mole in one move, so it is dominated by staying at the padded boundary
// -- padding by exactly d is therefore sufficient.
// dp[x][y] = best score achievable with hammer currently at (x,y) after
// processing all time steps done so far.  For time step t = 1..maxT (every
// integer time up to the largest one present, even times with no moles --
// they still grant a free repositioning move, per the discuss board), the
// hammer moves from some (x1,y1) to (x2,y2) with Euclidean distance <= d;
// it whacks every mole scheduled at time t lying on the segment between
// them (inclusive of endpoints).  The number of integer lattice points on a
// segment is gcd(|dx|,|dy|)+1 (also from the discuss board), so each
// transition is checked against at most d+1 <= 6 candidate points using a
// boolean occupancy grid for the current time step.
// Complexity per test: maxT(<=10) * (n+2d)^2(<=900) * disk-size(<=81) *
// (d+1)(<=6), a few million operations -- comfortably fast.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { int t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int n, d, m;
    while (scanf("%d %d %d", &n, &d, &m) == 3) {
        if (n == 0 && d == 0 && m == 0) break;
        int M = n + 2 * d; // extended grid side
        // read moles, bucket by time (1..10)
        static int mx[1005], my[1005], mt[1005];
        int maxT = 0;
        for (int i = 0; i < m; i++) {
            int x, y, t;
            scanf("%d %d %d", &x, &y, &t);
            mx[i] = x + d; // shift into extended grid
            my[i] = y + d;
            mt[i] = t;
            maxT = max(maxT, t);
        }
        // precompute disk offsets (dx,dy) with dx^2+dy^2 <= d^2
        static int offx[200], offy[200];
        int noff = 0;
        for (int dx = -d; dx <= d; dx++)
            for (int dy = -d; dy <= d; dy++)
                if (dx * dx + dy * dy <= d * d) { offx[noff] = dx; offy[noff] = dy; noff++; }

        // dp arrays, size M x M (M <= 30)
        static int dp[32][32], ndp[32][32];
        for (int x = 0; x < M; x++)
            for (int y = 0; y < M; y++) dp[x][y] = 0;

        static bool moleAt[32][32];
        for (int t = 1; t <= maxT; t++) {
            memset(moleAt, 0, sizeof(moleAt));
            for (int i = 0; i < m; i++)
                if (mt[i] == t) moleAt[mx[i]][my[i]] = true;

            for (int x2 = 0; x2 < M; x2++) {
                for (int y2 = 0; y2 < M; y2++) {
                    int best = -1;
                    for (int k = 0; k < noff; k++) {
                        int x1 = x2 - offx[k];
                        int y1 = y2 - offy[k];
                        if (x1 < 0 || x1 >= M || y1 < 0 || y1 >= M) continue;
                        int dxv = x2 - x1, dyv = y2 - y1;
                        int g = gcd(dxv, dyv);
                        int cnt = 0;
                        if (g == 0) {
                            cnt = moleAt[x1][y1] ? 1 : 0;
                        } else {
                            int sx = dxv / g, sy = dyv / g;
                            int cx = x1, cy = y1;
                            for (int step = 0; step <= g; step++) {
                                if (moleAt[cx][cy]) cnt++;
                                cx += sx; cy += sy;
                            }
                        }
                        int val = dp[x1][y1] + cnt;
                        if (val > best) best = val;
                    }
                    ndp[x2][y2] = best;
                }
            }
            for (int x = 0; x < M; x++)
                for (int y = 0; y < M; y++) dp[x][y] = ndp[x][y];
        }

        int ans = 0;
        for (int x = 0; x < M; x++)
            for (int y = 0; y < M; y++) ans = max(ans, dp[x][y]);
        printf("%d\n", ans);
    }
    return 0;
}
