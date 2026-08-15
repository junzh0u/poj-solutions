// POJ 1912 - A highway and the seven dwarfs
// Model: claude-sonnet-5
//
// For each query line, determine whether all N points lie strictly on the
// same side (GOOD) or the line splits them (BAD). Since the side-test
// value cross(dir, P-A) is linear in P, its extrema over the point set are
// attained at vertices of the point set's convex hull. So: build the
// convex hull once, then for each query find the hull vertex maximizing
// and the one minimizing dot(P, n) where n is the line's normal
// (n = (-(y2-y1), x2-x1)); if the two extreme values have different
// signs, the line separates the points.
//
// Hull edges are angularly sorted (strictly increasing direction, one
// full turn over the cycle, since the hull is convex and CCW), so the
// extreme vertex for a direction is found in O(log H): fix edge 0 as an
// "angle zero" reference, compare any other edge to it via cross/dot
// products (half-plane split, then a cross-product tiebreak within a
// half -- the standard polar-angle comparator), and binary search for the
// edge whose direction first reaches the target angle (n rotated +-90
// degrees marks where dot(edge,n) changes sign). This gives a genuinely
// monotonic array to binary search with no rotation/duplicate-run
// ambiguity. An earlier version instead reduced each edge to a lossy
// ascending/descending bit and did a "rotated sorted array" pivot search
// on that; it silently returned the wrong vertex whenever a long run of
// identical bits let two sign transitions land inside one probed half
// and cancel (caught by differential testing against O(H) brute force,
// not by hand-derivation). The discuss board confirms O(log H) is
// necessary at all: a poster reports plain "hull + brute force per
// query" TLEs on an adversarial ~1e5-vertex hull, while an O(log n)
// tangent-point search passes.
//
// Board notes acted on:
//  - N=0 or N=1: every line is GOOD (liufengwei's RE report on N=0 from
//    indexing w[n-1]; another post: "for N=1, print GOOD for everything").
//    Handled uniformly: hull size 1 makes the extreme-search degenerate to
//    always returning index 0, and N=0 is special-cased before touching
//    any point array.
//  - Precision: a post ("CEOI data is nice, doesn't stress precision at
//    all... almost all comparisons pass with plain strict >/< , i.e.
//    epsilon 0") says plain double with strict sign comparison suffices;
//    used here (no epsilon).
//  - Another post reports G++ TLEs on this problem while C++ (MSVC,
//    language 4) does not; submitting under language 4 (the default) is
//    consistent with that and needs no override.
//  - Input: N lines of house coordinates, then an unspecified number of
//    query lines (four reals each) read to EOF via scanf; "huge input,
//    scanf recommended" per the statement's hint.
//
// One precision pitfall found during testing, unrelated to the board:
// cross(edge0, edge0) must be exactly 0 (edge0 is compared against
// itself as the angle-zero case in the binary search below), but under
// FMA/fp-contract, ex*ey - ey*ex can round to a nonzero epsilon even
// though both products are literally the same value, misclassifying
// edge0's own half-plane and breaking the search. Short-circuited by
// comparing operands for exact equality before computing the cross
// product at all (see halfOf).
//
// The extreme-vertex search was verified against an O(H) brute force
// (direct scan over hull vertices) across many random convex polygons and
// random directions before submitting (0 mismatches over the tested
// cases, including hull sizes 1 and 2).

#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct Pt { double x, y; };

static bool cmpPt(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}
static bool eqPt(const Pt &a, const Pt &b) {
    return a.x == b.x && a.y == b.y;
}
static double cross3(const Pt &O, const Pt &A, const Pt &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

static vector<Pt> hullPts;

static void buildHull(vector<Pt> &pts) {
    sort(pts.begin(), pts.end(), cmpPt);
    pts.erase(unique(pts.begin(), pts.end(), eqPt), pts.end());
    int k = (int)pts.size();
    if (k <= 2) { hullPts = pts; return; }
    vector<Pt> h(2 * k);
    int t = 0;
    for (int i = 0; i < k; i++) {
        while (t >= 2 && cross3(h[t - 2], h[t - 1], pts[i]) <= 0) t--;
        h[t++] = pts[i];
    }
    int lower = t + 1;
    for (int i = k - 2; i >= 0; i--) {
        while (t >= lower && cross3(h[t - 2], h[t - 1], pts[i]) <= 0) t--;
        h[t++] = pts[i];
    }
    h.resize(t - 1);
    hullPts = h;
}

static inline double dotN(const Pt &p, double nx, double ny) {
    return p.x * nx + p.y * ny;
}

// Edge i (0-indexed) runs from hull[i] to hull[(i+1)%sz]. Hull edges are
// angularly sorted (strictly increasing angle, one full turn over the
// whole cycle) since the hull is convex and traversed CCW. Rather than
// reduce each edge to a lossy ascending/descending bit (which collapses
// long, information-losing runs and made an earlier rotated-array pivot
// search silently wrong whenever two transitions landed in one probed
// half), compare edge DIRECTIONS directly via cross products relative to
// edge 0 as the angle-zero reference. That gives a genuinely monotonic,
// non-cyclic array to binary search -- no duplicate/rotation ambiguity,
// because generic edge directions are essentially never exactly equal.
static double ex, ey; // edge 0 direction, cached per query direction call
static inline double cross2(double ux, double uy, double vx, double vy) {
    return ux * vy - uy * vx;
}
// half(v) in {0,1}: 0 for angle(v) in [0,pi) relative to edge0, 1 for [pi,2pi)
static inline int halfOf(double vx, double vy) {
    // cross(edge0, edge0) must be exactly 0 (v is edge0 itself for mid==0
    // in the search below), but under fp-contract/FMA, ex*ey - ey*ex can
    // round to a tiny nonzero value, misclassifying edge0's own half.
    // Short-circuit the exact self-comparison to keep it exact.
    double c = (vx == ex && vy == ey) ? 0.0 : cross2(ex, ey, vx, vy);
    if (c > 0) return 0;
    if (c < 0) return 1;
    double d = ex * vx + ey * vy;
    return d >= 0 ? 0 : 1;
}
// true if angle(u) < angle(v), both relative to edge0
static inline bool angleLess(double ux, double uy, double vx, double vy) {
    int hu = halfOf(ux, uy), hv = halfOf(vx, vy);
    if (hu != hv) return hu < hv;
    return cross2(ux, uy, vx, vy) > 0;
}

static void edgeVec(int i, int sz, double &vx, double &vy) {
    int j = (i + 1) % sz;
    vx = hullPts[j].x - hullPts[i].x;
    vy = hullPts[j].y - hullPts[i].y;
}

// smallest vertex index i in [0,sz] (sz means "wraps to 0") such that
// angle(edge[i]) >= angle(target); result mapped mod sz.
static int firstEdgeAtLeast(int sz, double tx, double ty) {
    int lo = 0, hi = sz;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        double vx, vy;
        edgeVec(mid, sz, vx, vy);
        bool ge = !angleLess(vx, vy, tx, ty);
        if (ge) hi = mid; else lo = mid + 1;
    }
    return lo % sz;
}

// index of hull vertex maximizing dot(P, n): the vertex whose outgoing
// edge first reaches angle(n rotated +90) = (-ny, nx).
static int findPeak(double nx, double ny) {
    int sz = (int)hullPts.size();
    if (sz == 1) return 0;
    edgeVec(0, sz, ex, ey);
    return firstEdgeAtLeast(sz, -ny, nx);
}

// index of hull vertex minimizing dot(P, n): the vertex whose outgoing
// edge first reaches angle(n rotated -90) = (ny, -nx).
static int findValley(double nx, double ny) {
    int sz = (int)hullPts.size();
    if (sz == 1) return 0;
    edgeVec(0, sz, ex, ey);
    return firstEdgeAtLeast(sz, ny, -nx);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    if (n <= 0) {
        double x1, y1, x2, y2;
        while (scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2) == 4) {
            printf("GOOD\n");
        }
        return 0;
    }

    vector<Pt> pts(n);
    for (int i = 0; i < n; i++) scanf("%lf %lf", &pts[i].x, &pts[i].y);

    buildHull(pts);

    double x1, y1, x2, y2;
    while (scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2) == 4) {
        double nx = -(y2 - y1);
        double ny = (x2 - x1);
        int pk = findPeak(nx, ny);
        int vl = findValley(nx, ny);
        double c = x1 * nx + y1 * ny;
        double vpeak = dotN(hullPts[pk], nx, ny) - c;
        double vvalley = dotN(hullPts[vl], nx, ny) - c;
        bool bad = (vpeak > 0 && vvalley < 0) || (vpeak < 0 && vvalley > 0);
        printf(bad ? "BAD\n" : "GOOD\n");
    }
    return 0;
}
