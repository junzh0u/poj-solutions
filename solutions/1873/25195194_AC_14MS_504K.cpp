// POJ 1873 - The Fortified Forest
// Model: claude-opus-5
//
// n <= 15, so enumerate all 2^n subsets of trees to cut.  A subset is feasible
// when the convex hull perimeter of the trees left standing does not exceed the
// total fence length yielded by the cut trees; the answer minimises (total cut
// value, number of cut trees) lexicographically, and the extra wood is the
// leftover fence length.
//
// Hull perimeter is Andrew's monotone chain with collinear points dropped.  The
// degenerate shapes are the ones the discuss board reports as WA-bait: a single
// remaining tree has perimeter 0, and a set of remaining trees that is collinear
// (two trees being the common case) is fenced by walking the segment in both
// directions, so its perimeter is twice its length.
//
// Speed: the point order and all pairwise distances are computed once per test
// case, so the per-subset work is a monotone chain over at most 15 points with
// table lookups and no sqrt; subsets that cannot beat the current best on
// (value, count) are rejected in O(1) from prefix-summed masks.
//
// Ambiguity: the statement stops tie-breaking at "smallest number of trees", and
// POJ has no special judge for this problem, so subsets tying on both value and
// count are still possible.  This picks the smallest cut bitmask (bit i = tree
// i+1), i.e. it prefers cutting lower-numbered trees, which is what the usual
// ascending-mask enumeration produces.
//
// Long long is avoided deliberately: POJ language 4 is an ancient MSVC.  Output
// uses %.2f rather than %.2lf for the same portability reason - the board
// reports %.2lf printing garbage under MinGW G++.

#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

const int MAXN = 15;

static int X[MAXN], Y[MAXN], V[MAXN], L[MAXN];
static int ord[MAXN];              /* tree indices sorted by (x, y) */
static double D[MAXN][MAXN];       /* pairwise distances, computed once */
static int sumV[1 << MAXN], sumL[1 << MAXN];
static unsigned char popc[1 << MAXN];

struct ByPos {
    bool operator()(int a, int b) const {
        if (X[a] != X[b]) return X[a] < X[b];
        return Y[a] < Y[b];
    }
};

/* double, not long long: POJ language 4 is an ancient MSVC without long long.
   Coordinates are small enough that this cross product stays exact. */
static double cross(int o, int a, int b) {
    return (double)(X[a] - X[o]) * (Y[b] - Y[o]) - (double)(Y[a] - Y[o]) * (X[b] - X[o]);
}

/* Perimeter of the convex hull of p[0..m), which must already be sorted by
   (x, y) and free of duplicate positions.  Andrew's monotone chain, with
   collinear points dropped.  A hull that degenerates to a segment is walked in
   both directions, so its perimeter is twice the segment length; a single
   point has perimeter 0. */
static double hullPerimeter(const int *p, int m) {
    if (m <= 1) return 0.0;
    int h[2 * MAXN + 1];
    int i, k = 0;
    for (i = 0; i < m; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], p[i]) <= 0.0) k--;
        h[k++] = p[i];
    }
    int lower = k + 1;
    for (i = m - 2; i >= 0; i--) {
        while (k >= lower && cross(h[k - 2], h[k - 1], p[i]) <= 0.0) k--;
        h[k++] = p[i];
    }
    /* h[0..k-1] closes the cycle: h[k-1] == h[0] */
    double per = 0.0;
    for (i = 0; i + 1 < k; i++) per += D[h[i]][h[i + 1]];
    return per;
}

int main() {
    int n, cas = 0;

    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j, s;
        for (i = 0; i < n; i++)
            scanf("%d %d %d %d", &X[i], &Y[i], &V[i], &L[i]);

        for (i = 0; i < n; i++) {
            D[i][i] = 0.0;
            for (j = 0; j < i; j++) {
                double dx = (double)(X[i] - X[j]);
                double dy = (double)(Y[i] - Y[j]);
                D[i][j] = D[j][i] = sqrt(dx * dx + dy * dy);
            }
        }

        for (i = 0; i < n; i++) ord[i] = i;
        sort(ord, ord + n, ByPos());

        int full = 1 << n;
        sumV[0] = sumL[0] = 0;
        popc[0] = 0;
        for (s = 1; s < full; s++) {
            int low = s & (-s), rest = s ^ low;
            int b = 0;
            while ((1 << b) != low) b++;
            sumV[s] = sumV[rest] + V[b];
            sumL[s] = sumL[rest] + L[b];
            popc[s] = (unsigned char)(popc[rest] + 1);
        }

        int bestMask = -1, bestVal = 0, bestCnt = 0;
        double bestExtra = 0.0;

        /* s is the set of trees cut; the full set is skipped because it leaves
           nothing to enclose (and cutting n-1 trees is always both feasible
           and no worse). */
        for (s = 0; s < full - 1; s++) {
            int cv = sumV[s], cnt = popc[s];
            if (bestMask >= 0 && (cv > bestVal || (cv == bestVal && cnt >= bestCnt)))
                continue;

            int rem[MAXN], m = 0;
            for (i = 0; i < n; i++) {
                int t = ord[i];
                if (s & (1 << t)) continue;
                if (m > 0 && X[rem[m - 1]] == X[t] && Y[rem[m - 1]] == Y[t]) continue;
                rem[m++] = t;
            }

            double per = hullPerimeter(rem, m);
            if (per > (double)sumL[s] + 1e-9) continue;

            double extra = (double)sumL[s] - per;
            if (extra < 0.0) extra = 0.0;
            bestMask = s;
            bestVal = cv;
            bestCnt = cnt;
            bestExtra = extra;
        }

        if (cas > 0) printf("\n");
        cas++;
        printf("Forest %d\n", cas);
        printf("Cut these trees:");
        for (i = 0; i < n; i++)
            if (bestMask & (1 << i)) printf(" %d", i + 1);
        printf(" \n");
        printf("Extra wood: %.2f\n", bestExtra);
    }
    return 0;
}
