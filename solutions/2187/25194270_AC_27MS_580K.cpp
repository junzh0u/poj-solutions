// POJ 2187 - Beauty Contest
//
// Given N <= 50,000 lattice points with |x|,|y| <= 10,000, print the squared
// distance of the farthest pair (the squared diameter of the point set).
//
// The diameter is always realised by two vertices of the convex hull, so:
//   1. Build the hull with Andrew's monotone chain, popping on cross <= 0 so
//      collinear points are dropped and the hull comes out strictly convex --
//      that is what the calipers loop below assumes.
//   2. Sweep it with rotating calipers: for each hull edge (i, i+1) advance the
//      antipodal vertex j while it keeps increasing the triangle area
//      cross(P[i], P[i+1], P[j]), i.e. while it moves further from the edge's
//      supporting line, and take the best of dist2(P[i],P[j]) and
//      dist2(P[i+1],P[j]). Total O(n log n), dominated by the sort.
//
// Degenerate hulls are handled outside the sweep: a 2-point hull (all input
// points collinear) is answered directly, and a 1-point hull (every point
// identical) gives 0.
//
// No floating point is needed anywhere -- the answer is asked for squared, and
// the largest value in play is 2 * 20000^2 = 8e8, which still fits in 32 bits;
// long long is used only to keep the cross products obviously safe. The result
// is printed digit by digit so no %lld / %I64d format ever appears, keeping the
// source portable across POJ's G++ and C++ compilers.
//
// Statement notes: it promises "no two farms share the same pair of
// coordinates", but the input is de-duplicated anyway since it costs nothing
// after the sort. The output is the *squared* distance, not the distance --
// the sample answer 2 for the unit square is the giveaway.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

struct Pt {
    int x, y;
};

static bool lessPt(const Pt& a, const Pt& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

static bool samePt(const Pt& a, const Pt& b) {
    return a.x == b.x && a.y == b.y;
}

static long long cross(const Pt& o, const Pt& a, const Pt& b) {
    return (long long)(a.x - o.x) * (b.y - o.y) - (long long)(a.y - o.y) * (b.x - o.x);
}

static long long dist2(const Pt& a, const Pt& b) {
    long long dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

/* Monotone chain; drops collinear points so the hull is strictly convex. */
static int convexHull(Pt* p, int n, Pt* h) {
    std::sort(p, p + n, lessPt);
    n = (int)(std::unique(p, p + n, samePt) - p);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    int lower = k + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (k >= lower && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    return n <= 1 ? n : k - 1;
}

static void writeLL(long long v) {
    char buf[24];
    int i = 0;
    if (v < 0) { putchar('-'); v = -v; }
    if (v == 0) buf[i++] = '0';
    while (v > 0) { buf[i++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (i > 0) putchar(buf[--i]);
    putchar('\n');
}

static Pt pts[50005];
static Pt hull[100015];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) scanf("%d %d", &pts[i].x, &pts[i].y);

    int m = convexHull(pts, n, hull);

    long long best = 0;
    if (m == 2) {
        best = dist2(hull[0], hull[1]);
    } else if (m > 2) {
        int j = 1;
        for (int i = 0; i < m; i++) {
            int ni = (i + 1) % m;
            while (cross(hull[i], hull[ni], hull[(j + 1) % m]) >
                   cross(hull[i], hull[ni], hull[j])) {
                j = (j + 1) % m;
            }
            long long a = dist2(hull[i], hull[j]);
            long long b = dist2(hull[ni], hull[j]);
            if (a > best) best = a;
            if (b > best) best = b;
        }
    }
    writeLL(best);
    return 0;
}
