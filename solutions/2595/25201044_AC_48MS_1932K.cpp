// POJ 2595 - Min-Max
// Model: claude-sonnet-5
//
// F(x1..xn) = sum(mu_i * x_i), where mu_i>=0, mu_i<=1, sum mu_i = 1 (a
// convex-combination weight vector, same mu used to evaluate F on both the
// p sequence and the q sequence). Given C = F(p), the set of achievable
// pairs (F(p), F(q)) over all valid mu is exactly the convex hull of the n
// points (p_i, q_i) - any point of a convex hull is some convex
// combination of the vertices (Caratheodory), and conversely every convex
// combination of the points lies in the hull. So we want, for x = C, the
// range of y such that (C, y) lies in the hull of the points (p_i, q_i):
// the min is the lower hull's y at x=C, the max is the upper hull's y at
// x=C, found by linear interpolation between the two hull vertices that
// bracket C.
//
// To keep the two hull chains strictly increasing in x (avoiding
// degenerate vertical segments at duplicate x-coordinates, which the
// board flags as a real trap), we first collapse points sharing an x to
// (x, min y) for the lower-hull input and (x, max y) for the upper-hull
// input, then run the standard monotone-chain construction on each.
//
// Multiple test cases run to EOF (per the board, a common WA source is
// forgetting this). C is guaranteed to be achievable, i.e. within
// [min p_i, max p_i], so no bounds check is needed beyond clamping the
// binary search.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct Pt {
    long long x, y;
};

static long long cross(const Pt &O, const Pt &A, const Pt &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Build a hull chain from points already sorted ascending by x (unique x).
// keepCondition: cross <= 0 pop, standard monotone chain (gives the lower
// hull when scanning left-to-right, or the upper hull when scanning
// right-to-left and then reversing).
static vector<Pt> buildLower(vector<Pt> pts) {
    vector<Pt> h;
    for (size_t i = 0; i < pts.size(); i++) {
        while (h.size() >= 2 && cross(h[h.size() - 2], h[h.size() - 1], pts[i]) <= 0)
            h.pop_back();
        h.push_back(pts[i]);
    }
    return h;
}

static vector<Pt> buildUpper(vector<Pt> pts) {
    vector<Pt> h;
    for (size_t i = pts.size(); i-- > 0;) {
        while (h.size() >= 2 && cross(h[h.size() - 2], h[h.size() - 1], pts[i]) <= 0)
            h.pop_back();
        h.push_back(pts[i]);
    }
    reverse(h.begin(), h.end());
    return h;
}

// Interpolate y at x=C along a hull chain with strictly increasing x.
static bool ptLess(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

static double evalChain(const vector<Pt> &h, long long C) {
    if (h.size() == 1) return (double)h[0].y;
    if (C <= h.front().x) return (double)h.front().y;
    if (C >= h.back().x) return (double)h.back().y;
    // binary search for the segment bracketing C
    int lo = 0, hi = (int)h.size() - 1;
    while (hi - lo > 1) {
        int mid = (lo + hi) / 2;
        if (h[mid].x <= C) lo = mid; else hi = mid;
    }
    double x1 = (double)h[lo].x, y1 = (double)h[lo].y;
    double x2 = (double)h[hi].x, y2 = (double)h[hi].y;
    if (x1 == x2) return y1; // shouldn't happen (unique x), defensive
    return y1 + (y2 - y1) * ((double)C - x1) / (x2 - x1);
}

int main() {
    int n;
    long long C;
    static long long p[50005], q[50005];
    while (scanf("%d %lld", &n, &C) == 2) {
        for (int i = 0; i < n; i++) scanf("%lld", &p[i]);
        for (int i = 0; i < n; i++) scanf("%lld", &q[i]);

        vector<Pt> pts(n);
        for (int i = 0; i < n; i++) { pts[i].x = p[i]; pts[i].y = q[i]; }
        sort(pts.begin(), pts.end(), ptLess);

        // Collapse duplicate x: lowIn keeps (x, min y), highIn keeps (x, max y).
        vector<Pt> lowIn, highIn;
        int i = 0;
        while (i < n) {
            int j = i;
            long long mny = pts[i].y, mxy = pts[i].y;
            while (j < n && pts[j].x == pts[i].x) {
                mny = min(mny, pts[j].y);
                mxy = max(mxy, pts[j].y);
                j++;
            }
            Pt lo; lo.x = pts[i].x; lo.y = mny;
            Pt hi; hi.x = pts[i].x; hi.y = mxy;
            lowIn.push_back(lo);
            highIn.push_back(hi);
            i = j;
        }

        vector<Pt> lowerHull = buildLower(lowIn);
        vector<Pt> upperHull = buildUpper(highIn);

        double mn = evalChain(lowerHull, C);
        double mx = evalChain(upperHull, C);
        if (mn == 0.0) mn = 0.0; // normalize -0
        if (mx == 0.0) mx = 0.0;
        printf("%.3f %.3f\n", mn, mx);
    }
    return 0;
}
