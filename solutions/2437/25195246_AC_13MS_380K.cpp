// POJ 2437 - Muddy roads
// Model: claude-opus-5
//
// Cover a set of disjoint segments [s_i, e_i] with as few length-L planks as
// possible.  Planks may overlap and may hang off the ends, so a plank is just
// an interval [p, p+L] placed anywhere.
//
// Greedy: sort the pools by start point and sweep left to right, keeping
// `covered` = the rightmost coordinate already under some plank.  For a pool
// starting at x = max(s, covered), the first plank must start no further left
// than x (otherwise x stays uncovered) and starting it exactly at x is optimal
// (pushing it right leaves a hole, pushing it left covers strictly less new
// road).  So the pool needs ceil((e-x)/L) planks laid end to end from x, and
// the new coverage reaches x + k*L, which may spill into later pools.
//
// The input is NOT sorted (the sample itself is 1..6, 13..17, 8..12), which is
// the pitfall the discuss board keeps hitting.  The other one is running the
// plank placement one plank at a time: with L=1 and coordinates to 1e9 that is
// 1e9 iterations, hence the "runs for hundreds of seconds" posts.  Computing
// the count per pool with one division keeps it O(N log N).
//
// Arithmetic stays in int: the answer is at most (total mud length)/L + N <=
// 1e9 + 1e4, and the running coverage is clamped at CAP = 1e9+1 (larger than
// any coordinate, so the clamp is invisible) to keep x + k*L from overflowing
// for large L.

#include <cstdio>
#include <algorithm>

struct Pool {
    int s, e;
};

static bool byStart(const Pool& a, const Pool& b) {
    if (a.s != b.s) return a.s < b.s;
    return a.e < b.e;
}

static Pool pools[10005];

int main() {
    int n, L;
    if (scanf("%d %d", &n, &L) != 2) return 0;
    for (int i = 0; i < n; ++i)
        scanf("%d %d", &pools[i].s, &pools[i].e);

    std::sort(pools, pools + n, byStart);

    const int CAP = 1000000001; /* strictly greater than any coordinate */
    int covered = -1;           /* nothing covered yet; coordinates are >= 0 */
    int count = 0;

    for (int i = 0; i < n; ++i) {
        int e = pools[i].e;
        int x = pools[i].s > covered ? pools[i].s : covered;
        if (x >= e) continue;

        int gap = e - x;
        int k = gap / L;
        if (gap % L != 0) ++k;
        count += k;

        if (k > (CAP - x) / L) covered = CAP;
        else covered = x + k * L;
    }

    printf("%d\n", count);
    return 0;
}
