// POJ 3608 - Bridge Across Islands
// Model: claude-opus-5
//
// Minimum distance between two convex polygons, via the Minkowski difference.
//
//   dist(A, B) = min_{a in A, b in B} |a - b| = dist(origin, A (+) (-B))
//
// where (+) is the Minkowski sum.  The sum of two convex polygons is a convex
// polygon whose edge vectors are exactly the union of the two edge multisets,
// sorted by direction angle; its lexicographically smallest vertex (lowest,
// then leftmost) is the sum of the two lexicographically smallest vertices.
// Walking the merged edge list from there rebuilds the sum in O(n + m).  The
// answer is then the distance from the origin to that polygon: 0 if the origin
// is inside it (the islands overlap), else the min over its edges of the
// point-to-segment distance.
//
// Key detail: for a CCW convex polygon started at its lowest-then-leftmost
// vertex, the edge directions are already sorted ascending as absolute angles
// in [0, 2*pi).  So the merge is a plain two-list merge with an angular
// comparator -- no wrap-around bookkeeping and no chance of the classic
// rotating-calipers / cross-product-advance loop getting stuck.
//
// The statement promises convex polygons, but the discuss board reports the
// real data contains scrambled vertex order, repeated points and points that
// are not hull vertices (and codes that skip the hull "no matter what could not
// pass").  So both point sets are re-hulled here (monotone chain, collinear
// points dropped) and the hull's own vertex count is used from then on.
// Orientation of the input therefore does not matter either.
//
// The board also reports G++ (language 0) printing doubles with "%.5lf" gives
// WA where "%.5f" is AC; submitted under language 4 (C++) with "%.5f".
// Input is parsed by hand, likewise: several posts report scanf("%lf") failing
// under G++ on this problem.
//
// Ambiguity: "the minimal distance between the boundaries" and the distance
// between the two regions differ in exactly one configuration -- one island
// strictly inside the other, where the boundaries are a positive distance apart
// but the regions overlap.  This code answers 0 there.  The statement calls the
// islands separated, so the case should not arise; a randomized differential
// test against a boundary-distance brute force found no other disagreement, and
// both agree on every case with a published answer (the sample, and the two
// data sets on the discuss board: 5 cases in message 170223 and 240 cases in
// message 344253).
//
// O((N + M) log(N + M)) per test case, dominated by the two hulls.

#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

static const double EPS = 1e-9;

struct Pt {
    double x, y;
};

static inline double crossO(const Pt &o, const Pt &a, const Pt &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

/* ---------- input ---------- */

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static bool readDouble(double &out) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    else if (c == '+') { c = gc(); }
    double v = 0.0;
    while (c >= '0' && c <= '9') { v = v * 10.0 + (c - '0'); c = gc(); }
    if (c == '.') {
        c = gc();
        double f = 0.0, p = 1.0;
        while (c >= '0' && c <= '9') { f = f * 10.0 + (c - '0'); p *= 10.0; c = gc(); }
        v += f / p;
    }
    out = sgn * v;
    return true;
}

static bool readInt(int &out) {
    double d;
    if (!readDouble(d)) return false;
    out = (int)(d < 0 ? d - 0.5 : d + 0.5);
    return true;
}

/* ---------- geometry ---------- */

static bool byXY(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// Andrew monotone chain, CCW, collinear points removed.
static int hull(Pt *p, int n, Pt *h) {
    if (n <= 0) return 0;
    sort(p, p + n, byXY);
    if (n == 1) { h[0] = p[0]; return 1; }
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && crossO(h[k - 2], h[k - 1], p[i]) <= EPS) k--;
        h[k++] = p[i];
    }
    int lower = k + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (k >= lower && crossO(h[k - 2], h[k - 1], p[i]) <= EPS) k--;
        h[k++] = p[i];
    }
    k--;                                   // last point == first point
    if (k == 2 && h[0].x == h[1].x && h[0].y == h[1].y) k = 1;
    return k;
}

// index of the lowest-then-leftmost vertex
static int lexMin(const Pt *p, int n) {
    int b = 0;
    for (int i = 1; i < n; i++)
        if (p[i].y < p[b].y || (p[i].y == p[b].y && p[i].x < p[b].x)) b = i;
    return b;
}

static inline int angHalf(const Pt &v) {
    return (v.y < 0 || (v.y == 0 && v.x < 0)) ? 1 : 0;
}

// -1: dir(a) before dir(b) as absolute angles in [0, 2*pi); 0: same; 1: after
static inline int cmpDir(const Pt &a, const Pt &b) {
    int ha = angHalf(a), hb = angHalf(b);
    if (ha != hb) return ha < hb ? -1 : 1;
    double c = a.x * b.y - a.y * b.x;
    if (c > 0) return -1;
    if (c < 0) return 1;
    return 0;
}

// distance from the origin to segment ab
static inline double distSegOrigin(const Pt &a, const Pt &b) {
    double dx = b.x - a.x, dy = b.y - a.y;
    double len2 = dx * dx + dy * dy;
    double t = 0.0;
    if (len2 > 0.0) {
        t = -(a.x * dx + a.y * dy) / len2;
        if (t < 0.0) t = 0.0;
        else if (t > 1.0) t = 1.0;
    }
    double px = a.x + t * dx, py = a.y + t * dy;
    return sqrt(px * px + py * py);
}

static bool segIntersect(const Pt &a, const Pt &b, const Pt &c, const Pt &d) {
    double d1 = crossO(a, b, c), d2 = crossO(a, b, d);
    double d3 = crossO(c, d, a), d4 = crossO(c, d, b);
    return ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
           ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0));
}

// distance from point p to segment ab
static inline double distPointSeg(const Pt &p, const Pt &a, const Pt &b) {
    Pt u, v;
    u.x = a.x - p.x; u.y = a.y - p.y;
    v.x = b.x - p.x; v.y = b.y - p.y;
    return distSegOrigin(u, v);
}

static double distSegSeg(const Pt &a, const Pt &b, const Pt &c, const Pt &d) {
    if (segIntersect(a, b, c, d)) return 0.0;
    double best = distPointSeg(c, a, b);
    double t = distPointSeg(d, a, b); if (t < best) best = t;
    t = distPointSeg(a, c, d); if (t < best) best = t;
    t = distPointSeg(b, c, d); if (t < best) best = t;
    return best;
}

/* ---------- main ---------- */

static const int MAXN = 10005;

static Pt A[MAXN], B[MAXN], HA[MAXN], HB[MAXN], NB[MAXN], R[2 * MAXN];

// Minkowski sum of two convex CCW polygons; result CCW, starting at its
// lowest-then-leftmost vertex.  Returns the vertex count.
static int minkowski(const Pt *P, int n, const Pt *Q, int m, Pt *out) {
    int ia = lexMin(P, n), ib = lexMin(Q, m);
    Pt cur;
    cur.x = P[ia].x + Q[ib].x;
    cur.y = P[ia].y + Q[ib].y;
    int i = 0, j = 0, k = 0;
    while (i < n || j < m) {
        out[k++] = cur;
        Pt ea, eb;
        if (i < n) {
            ea.x = P[(ia + i + 1) % n].x - P[(ia + i) % n].x;
            ea.y = P[(ia + i + 1) % n].y - P[(ia + i) % n].y;
        }
        if (j < m) {
            eb.x = Q[(ib + j + 1) % m].x - Q[(ib + j) % m].x;
            eb.y = Q[(ib + j + 1) % m].y - Q[(ib + j) % m].y;
        }
        int c;
        if (i >= n) c = 1;
        else if (j >= m) c = -1;
        else c = cmpDir(ea, eb);
        if (c <= 0) { cur.x += ea.x; cur.y += ea.y; i++; }
        if (c >= 0) { cur.x += eb.x; cur.y += eb.y; j++; }
    }
    return k;
}

// distance from the origin to a convex CCW polygon (0 if the origin is inside)
static double distOriginToConvex(const Pt *p, int n) {
    if (n == 1) return sqrt(p[0].x * p[0].x + p[0].y * p[0].y);
    if (n == 2) return distSegOrigin(p[0], p[1]);
    bool inside = true;
    double best = 1e100;
    for (int i = 0; i < n; i++) {
        const Pt &a = p[i];
        const Pt &b = p[(i + 1) % n];
        // cross(b - a, origin - a)
        if ((b.x - a.x) * (0.0 - a.y) - (b.y - a.y) * (0.0 - a.x) < 0.0) inside = false;
        double d = distSegOrigin(a, b);
        if (d < best) best = d;
    }
    return inside ? 0.0 : best;
}

// fallback for degenerate hulls (a single point or a segment): min distance
// between the two boundaries, brute force over the (very few) edge pairs
static double bruteBoundary(const Pt *p, int n, const Pt *q, int m) {
    double best = 1e100;
    int en = (n <= 2 ? 1 : n), em = (m <= 2 ? 1 : m);
    for (int i = 0; i < en; i++) {
        Pt a = p[i], b = p[n == 1 ? 0 : (i + 1) % n];
        for (int j = 0; j < em; j++) {
            Pt c = q[j], d = q[m == 1 ? 0 : (j + 1) % m];
            double v = distSegSeg(a, b, c, d);
            if (v < best) best = v;
        }
    }
    return best;
}

int main() {
    int n, m;
    while (readInt(n) && readInt(m)) {
        if (n == 0 && m == 0) break;
        for (int i = 0; i < n; i++) { readDouble(A[i].x); readDouble(A[i].y); }
        for (int i = 0; i < m; i++) { readDouble(B[i].x); readDouble(B[i].y); }

        int na = hull(A, n, HA);
        int nb = hull(B, m, HB);

        double ans;
        if (na <= 0 || nb <= 0) {
            ans = 0.0;                     // cannot happen: 3 <= N, M
        } else if (na < 3 || nb < 3) {
            ans = bruteBoundary(HA, na, HB, nb);
        } else {
            for (int i = 0; i < nb; i++) { NB[i].x = -HB[i].x; NB[i].y = -HB[i].y; }
            int nr = minkowski(HA, na, NB, nb, R);
            ans = distOriginToConvex(R, nr);
        }
        printf("%.5f\n", ans);
    }
    return 0;
}
