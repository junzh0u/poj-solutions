// POJ 1228 - Grandpa's Estate
//
// The spikes all sat on the boundary of a convex polygon; decide whether that
// polygon is uniquely recoverable from the spikes that remain ("stable").
//
// A convex polygon P having every spike on its boundary must contain the convex
// hull of the spikes.  If some hull edge AB carries no spike other than A and B,
// the edge can be pushed outward into a path A-C-B without moving any spike off
// the boundary, so a second polygon exists and the answer is NO.  If instead a
// spike lies strictly between A and B, that spike forces line AB to be a
// supporting line of P, pinning the edge.  So the hull is stable exactly when
// every one of its edges carries at least three spikes.
//
// The check: deduplicate (two spikes in one hole are one point), build the hull
// keeping only true corners, then for each edge count the input points that are
// collinear with it and inside its bounding box.  Any edge with fewer than three
// makes the answer NO.  All arithmetic is integer cross products.
//
// Degenerate input is the whole difficulty, and the statement does not exclude
// it: n can be as small as 1, and nothing forbids repeated or collinear spikes.
// Fewer than six distinct spikes can never be stable (three sides, three points
// each, corners shared), and a set lying entirely on one line bounds no polygon
// at all -- both are NO, and both are decided before the hull is built, since a
// hull routine would otherwise report a degenerate "edge" that looks satisfied.
//
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct P {
    long long x, y;
};

static bool lessP(const P &a, const P &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}
static bool eqP(const P &a, const P &b) { return a.x == b.x && a.y == b.y; }

static long long cross3(const P &o, const P &a, const P &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

/* Strict convex hull (corner vertices only), counter-clockwise, no repeated
   endpoint.  Input must be sorted and deduplicated. */
static void hull(const vector<P> &p, vector<P> &h) {
    int m = (int)p.size();
    h.assign(2 * m + 1, P());
    int k = 0;
    for (int i = 0; i < m; i++) {
        while (k >= 2 && cross3(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    int lower = k + 1;
    for (int i = m - 2; i >= 0; i--) {
        while (k >= lower && cross3(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    h.resize(k - 1);
}

static bool solve(vector<P> &p) {
    sort(p.begin(), p.end(), lessP);
    p.erase(unique(p.begin(), p.end(), eqP), p.end());
    int m = (int)p.size();

    /* A stable polygon has at least three sides and at least three points on
       each side, sharing the corners: six distinct spikes minimum. */
    if (m < 6) return false;

    /* All spikes on one line: no polygon at all, so nothing is determined. */
    bool collinear = true;
    for (int i = 2; i < m; i++)
        if (cross3(p[0], p[1], p[i]) != 0) { collinear = false; break; }
    if (collinear) return false;

    vector<P> h;
    hull(p, h);
    int k = (int)h.size();
    if (k < 3) return false;

    for (int i = 0; i < k; i++) {
        const P &a = h[i];
        const P &b = h[(i + 1) % k];
        long long lo_x = a.x < b.x ? a.x : b.x, hi_x = a.x < b.x ? b.x : a.x;
        long long lo_y = a.y < b.y ? a.y : b.y, hi_y = a.y < b.y ? b.y : a.y;
        int cnt = 0;
        for (int j = 0; j < m; j++) {
            const P &q = p[j];
            if (q.x < lo_x || q.x > hi_x || q.y < lo_y || q.y > hi_y) continue;
            if (cross3(a, b, q) == 0) cnt++;
        }
        if (cnt < 3) return false;
    }
    return true;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n;
        if (scanf("%d", &n) != 1) return 0;
        vector<P> p(n);
        for (int i = 0; i < n; i++) {
            int x, y;
            if (scanf("%d %d", &x, &y) != 2) return 0;
            p[i].x = x;
            p[i].y = y;
        }
        puts(solve(p) ? "YES" : "NO");
    }
    return 0;
}
