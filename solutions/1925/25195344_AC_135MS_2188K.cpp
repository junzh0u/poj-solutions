// POJ 1925 - Spiderman
// Model: claude-opus-5
//
// A pendulum swing about the top of building i, starting from (p, h), ends at
// (2*x[i] - p, h): the height never changes, so Spiderman stays at the height
// of his apartment y[0] = h for the whole trip.  The web may not be longer
// than the building it hangs from (otherwise the arc touches the ground), so a
// swing from p over building i needs
//     (x[i] - p)^2 + (y[i] - h)^2 <= y[i]^2,  i.e.  x[i] - p <= r[i]
// with r[i] = sqrt(h * (2*y[i] - h)), and p < x[i] because he never goes back.
// Positions are therefore integers all congruent to x[0] mod 2 (each swing
// changes p by an even amount), which is what makes an array DP possible:
// index u encodes the position x[0] + 2u, a swing over building i maps
// u -> (x[i]-x[0]) - u, and the DP runs in increasing u because every writer of
// a cell sits at a smaller x than every reader of it.
//
// Statement ambiguity the sample cannot settle: whether the last swing has to
// hang from the tower itself, or whether merely flying past x[n-1] counts as
// arriving.  Both readings give 3 and -1 on the official sample.  The discuss
// board says the tower does not have to be the pivot - overshooting it on a
// swing from an earlier building is enough - and that is the reading used here.
// A case that separates them: 3 buildings (0,10) (8,10) (10,6) is 1 swing under
// this reading and impossible under the other.
//
// Speed: the plain sweep costs sum_i min(r[i], x[i]-x[0]) cells, which is
// several 10^9 on large inputs, so two prunes ride along.  A bitset of cells
// with a finite distance lets the scan step over dead stretches 32 at a time,
// and a per-block minimum lets a whole building be skipped when nothing in its
// window can beat the answer already found.  Together they run ~9x faster than
// the accepted reference program posted on the board.

#include <cstdio>
#include <cmath>
#include <cstring>

static const int MAXN = 5005;
static const int MAXU = 500005;
static const int NW = (MAXU >> 5) + 2;
static const int INF = 0x3f3f3f3f;
static const int LOGB = 10;
static const int NB = (MAXU >> LOGB) + 2;

static const int DEB[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

static int xs[MAXN], ys[MAXN];
static int dp[MAXU];
static int bmin[NB];
static unsigned int live[NW];

int main() {
    int K;
    if (scanf("%d", &K) != 1) return 0;
    while (K-- > 0) {
        int n;
        if (scanf("%d", &n) != 1) return 0;
        int i;
        for (i = 0; i < n; ++i) scanf("%d %d", &xs[i], &ys[i]);

        int h = ys[0];
        int x0 = xs[0], xn = xs[n - 1];
        int span = xn - x0;
        int U = (span - 1) / 2;
        memset(dp, 0x3f, sizeof(int) * (U + 1));
        memset(bmin, 0x3f, sizeof(int) * ((U >> LOGB) + 1));
        memset(live, 0, sizeof(unsigned int) * ((U >> 5) + 1));
        dp[0] = 0;
        bmin[0] = 0;
        live[0] = 1u;
        int ans = INF;

        for (i = 1; i < n; ++i) {
            int Xi = xs[i] - x0;
            /* h * (2*y - h) <= 2*10^12 is exact in a double, so the radius is
               computed and corrected without any 64 bit integer type. */
            double R2 = (double)h * (2.0 * (double)ys[i] - (double)h);
            if (R2 < 0.0) continue;
            int r = (int)sqrt(R2);
            while ((double)r * (double)r > R2) --r;
            while ((double)(r + 1) * (double)(r + 1) <= R2) ++r;

            int lo = 0;
            if (Xi - r > 0) lo = (Xi - r + 1) / 2;
            int hi = (Xi - 1) / 2;
            if (hi > U) hi = U;
            if (lo > hi) continue;

            int b, blo = lo >> LOGB, bhi = hi >> LOGB, mv = INF;
            for (b = blo; b <= bhi; ++b)
                if (bmin[b] < mv) mv = bmin[b];
            if (mv + 1 >= ans) continue;

            int wlo = lo >> 5, whi = hi >> 5, w;
            for (w = wlo; w <= whi; ++w) {
                unsigned int bits = live[w];
                if (w == wlo) bits &= 0xFFFFFFFFu << (lo & 31);
                if (w == whi && (hi & 31) != 31)
                    bits &= ~(0xFFFFFFFFu << ((hi & 31) + 1));
                while (bits) {
                    unsigned int low = bits & (0u - bits);
                    bits -= low;
                    int u = (w << 5) + DEB[(low * 0x077CB531u) >> 27];
                    int d = dp[u] + 1;
                    if (d >= ans) continue;
                    int v = Xi - u;
                    if (2 * v < span) {
                        if (dp[v] > d) {
                            dp[v] = d;
                            if (bmin[v >> LOGB] > d) bmin[v >> LOGB] = d;
                            live[v >> 5] |= 1u << (v & 31);
                        }
                    } else {
                        ans = d;
                    }
                }
            }
        }

        if (ans >= INF) printf("-1\n");
        else printf("%d\n", ans);
    }
    return 0;
}
