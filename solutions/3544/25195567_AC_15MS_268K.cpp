// POJ 3544 - Journey with Pigs
// Model: claude-opus-5
//
// Selling pig i in village j earns w_i * p_j and burns t * w_i * d_j rubles of
// fuel, because that pig is carried exactly d_j kilometers.  The cost is
// therefore additive over pigs and independent of the visiting order, so the
// total is sum_j w_{sigma(j)} * c_j with c_j = p_j - t * d_j.  Maximizing a sum
// of pairwise products over all pairings is the rearrangement inequality: sort
// both sequences ascending and pair them position by position.  c_j is freely
// negative, which the inequality allows.
//
// Ranges: t * d_j reaches 10^18, so c_j must be 64-bit.  Every input value fits
// in a signed 32-bit int (all bounds are 10^9), so reading with %d avoids POJ's
// %lld / %I64d ambiguity entirely; only the product is widened.  The total
// profit itself would overflow 64 bits at n = 1000, but it is never needed.
//
// Output ambiguity: a discuss-board post claims the answer must be printed as
// "for pig j, its village" rather than the statement's "for village j, its pig".
// The official sample is invariant under the swap (its optimal pairing is an
// involution), so it cannot settle the question.  The NEERC 2007 Northern jury
// data does: 19 of its 20 answers are non-involutive, and the inverted printing
// disagrees with all 19.  The statement's reading stands.
//
// Ties (equal weights or equal c) admit several optimal answers, and POJ has no
// special judge here, so both sorts break ties by ascending input index --- the
// jury's own rule, which reproduces all 20 official answer files exactly.  No
// official test actually contains a duplicate weight or a duplicate c.

#include <cstdio>
#include <algorithm>

using namespace std;

static const int MAXN = 1005;

static int n;
static int w[MAXN], d[MAXN], p[MAXN];
static long long c[MAXN];
static int ordW[MAXN], ordC[MAXN], ans[MAXN];

struct CmpW {
    bool operator()(int a, int b) const {
        if (w[a] != w[b]) return w[a] < w[b];
        return a < b;
    }
};

struct CmpC {
    bool operator()(int a, int b) const {
        if (c[a] != c[b]) return c[a] < c[b];
        return a < b;
    }
};

int main() {
    int ti;
    /* The statement describes one data set; looping to EOF costs nothing and
       covers a file that turns out to hold several. */
    while (scanf("%d %d", &n, &ti) == 2) {
        long long t = ti;
        int i;
        for (i = 0; i < n; ++i) scanf("%d", &w[i]);
        for (i = 0; i < n; ++i) scanf("%d", &d[i]);
        for (i = 0; i < n; ++i) scanf("%d", &p[i]);
        for (i = 0; i < n; ++i) c[i] = (long long)p[i] - t * (long long)d[i];
        for (i = 0; i < n; ++i) { ordW[i] = i; ordC[i] = i; }
        sort(ordW, ordW + n, CmpW());
        sort(ordC, ordC + n, CmpC());
        /* i-th lightest pig goes to the i-th least profitable village. */
        for (i = 0; i < n; ++i) ans[ordC[i]] = ordW[i];
        for (i = 0; i < n; ++i) {
            if (i) putchar(' ');
            printf("%d", ans[i] + 1);
        }
        putchar('\n');
    }
    return 0;
}
