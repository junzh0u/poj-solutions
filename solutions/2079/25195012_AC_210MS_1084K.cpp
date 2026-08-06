// POJ 2079 - Triangle
// Model: claude-sonnet-5
// Approach: For each test case, build the convex hull of the n points (Andrew
// monotone chain, O(n log n)), then find the maximum-area triangle whose
// vertices are hull vertices. The maximum-area triangle among all n points is
// always formed by three hull vertices (any point strictly inside the hull,
// or any point that is not itself extremal, cannot be part of the maximum
// triangle), so restricting to the hull is safe and shrinks n (up to 50000)
// down to h, the hull size.
//
// For the hull (h vertices, CCW, doubled into a length-2h array to avoid
// modulo arithmetic), enumerate a fixed vertex i, then walk j forward from
// i+1; for each j the third vertex k that maximizes area(i,j,k) only moves
// forward as j increases (area(i,j,k) is unimodal in k for a convex polygon),
// so a single two-pointer sweep over j and k (reset for each i) costs O(h)
// per i, giving O(h^2) total. This is deliberately NOT the "fully O(h)"
// rotating-calipers variant that lets k/j persist across the outer i loop
// too: POJ's discuss board for this problem documents that global variant as
// broken (unproven / has counterexamples), while the per-i-reset O(h^2)
// version is the one widely confirmed AC. The board also notes real test
// data has driven the hull size up to ~2596 points, so O(h^2) (~6.7M ops)
// comfortably fits the 3000ms limit, while a naive O(n^2) over all 50000
// points would not.
//
// All coordinates are integers in [-10^4, 10^4], so twice the signed area of
// any triangle is an exact integer representable in long long (bounded by
// ~8*10^8) - the whole search is done in exact integer arithmetic, only
// converting to a double (divide by 2) for the final printed answer, which
// avoids any floating point precision issues in comparisons.
//
// Statement note: earlier discuss-board reports mention solvers who wrongly
// assumed the optimal triangle uses only *edges* of the convex hull (i.e.
// adjacent hull vertices); it does not need to - the algorithm above checks
// all pairs of hull vertices (i, j), not just hull edges, which is required
// for correctness.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef long long ll;

struct Pt {
    ll x, y;
};

static Pt pts[50005];
static Pt hull[50005];
static Pt hull2[100010];

bool cmpPt(const Pt &a, const Pt &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

ll cross(const Pt &O, const Pt &A, const Pt &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Andrew monotone chain convex hull, CCW, strict (no collinear points kept).
int buildHull(int n) {
    sort(pts, pts + n, cmpPt);
    int m = 0;
    // lower hull
    for (int i = 0; i < n; i++) {
        while (m >= 2 && cross(hull[m - 2], hull[m - 1], pts[i]) <= 0) m--;
        hull[m++] = pts[i];
    }
    // upper hull
    int lower = m + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (m >= lower && cross(hull[m - 2], hull[m - 1], pts[i]) <= 0) m--;
        hull[m++] = pts[i];
    }
    m--; // last point == first point
    return m;
}

ll area2(const Pt &a, const Pt &b, const Pt &c) {
    ll v = cross(a, b, c);
    return v < 0 ? -v : v;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != -1) {
        for (int i = 0; i < n; i++) {
            long long x, y;
            scanf("%lld %lld", &x, &y);
            pts[i].x = x;
            pts[i].y = y;
        }

        int h = 0;
        if (n >= 3) h = buildHull(n);

        ll best = 0;
        if (h >= 3) {
            for (int t = 0; t < h; t++) {
                hull2[t] = hull[t];
                hull2[t + h] = hull[t];
            }
            for (int i = 0; i < h; i++) {
                int k = i + 2;
                for (int j = i + 1; j <= i + h - 2; j++) {
                    if (k <= j) k = j + 1;
                    while (k + 1 <= i + h - 1 &&
                           area2(hull2[i], hull2[j], hull2[k + 1]) >
                               area2(hull2[i], hull2[j], hull2[k])) {
                        k++;
                    }
                    ll a = area2(hull2[i], hull2[j], hull2[k]);
                    if (a > best) best = a;
                }
            }
        }

        double ans = best / 2.0;
        printf("%.2f\n", ans);
    }
    return 0;
}
