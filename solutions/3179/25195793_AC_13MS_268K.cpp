// POJ 3179 - Corral the Cows
// Model: claude-opus-5
//
// Binary search the side length L; feasibility is a two-pointer sweep.
//
// Ambiguity settled by the statement's exact words plus the sample: a clover
// field is not a point but the unit block [x,x+1] x [y,y+1], and the corral
// "surrounds" it only if it lies entirely inside.  A square [a,a+L] x [b,b+L]
// therefore holds the field iff a <= x && x+1 <= a+L, so the window on the
// lower-left corners is the CLOSED interval [a, a+L-1] -- width L-1, not L.
// The sample separates the two readings (it answers 4; the point reading
// answers 3), which is why it is real evidence here and not decoration.
//
// feasible(L): the optimal square can slide right until its left edge meets
// some field's x, so only the distinct x values are candidate left edges.  For
// each, walk the y-sorted points, keep those inside the x-window (order is
// preserved, so no re-sort), and two-pointer for a y-window of width L-1
// holding >= C of them.  O(N^2) per check, ~14 checks: instant at N = 500.
//
// Duplicate coordinates count once each, per the statement and the board; the
// sweep never dedupes points, only the candidate x list.  All arithmetic is
// exact integer and bounded by 10^4, so 32-bit int suffices everywhere.
#include <cstdio>
#include <algorithm>
using namespace std;

int C, N;
int px[505], py[505];      /* points sorted by y */
int xs[505]; int nxs;      /* distinct x values, sorted */
int buf[505];

/* Is there an axis-parallel square of side L containing at least C fields?
   A field is the unit block [x,x+1]x[y,y+1]; a square [a,a+L]x[b,b+L] holds it
   entirely iff a <= x && x+1 <= a+L, i.e. corner x in [a, a+L-1].  So the
   window on lower-left corners is closed of width L-1. */
static bool feasible(int L) {
    for (int t = 0; t < nxs; ++t) {
        int a = xs[t], b = a + L - 1;
        int m = 0;
        for (int k = 0; k < N; ++k)
            if (px[k] >= a && px[k] <= b) buf[m++] = py[k];  /* stays y-sorted */
        int i = 0;
        for (int j = 0; j < m; ++j) {
            while (buf[j] - buf[i] > L - 1) ++i;
            if (j - i + 1 >= C) return true;
        }
    }
    return false;
}

int main() {
    if (scanf("%d %d", &C, &N) != 2) return 0;
    static pair<int, int> p[505];   /* (y, x) so sorting orders by y */
    for (int i = 0; i < N; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        p[i] = make_pair(b, a);
    }
    sort(p, p + N);
    for (int i = 0; i < N; ++i) { py[i] = p[i].first; px[i] = p[i].second; }

    for (int i = 0; i < N; ++i) xs[i] = px[i];
    sort(xs, xs + N);
    nxs = (int)(unique(xs, xs + N) - xs);

    int lo = 1, hi = 10000;         /* side 10000 spans every legal coordinate */
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(mid)) hi = mid; else lo = mid + 1;
    }
    printf("%d\n", lo);
    return 0;
}
