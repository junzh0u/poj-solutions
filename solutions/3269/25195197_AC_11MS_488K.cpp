// POJ 3269 - Building A New Barn
// Model: claude-opus-5
//
// Manhattan cost separates: f(X,Y) = g(X) + h(Y) with g(X) = sum|X-Xi|,
// h(Y) = sum|Y-Yi|.  Each is convex piecewise linear and is minimized exactly
// on the median interval, so the unconstrained optimum is the whole rectangle
// [xlo,xhi] x [ylo,yhi] with xlo = xs[(n-1)/2], xhi = xs[n/2] (0-indexed on the
// sorted coordinate multiset), likewise for y.  Odd n just collapses this to a
// single point, so both parities are one case.
//
// The barn may not stand on a grazing spot, so the answer is
//   (area of the rectangle) - (distinct cows inside it),
// counting cows *distinct* because the statement forbids two spots being
// horizontally or vertically adjacent but does not forbid two cows sharing a
// spot; the cost sum still uses the full multiset.
//
// When that count is 0 the whole optimal rectangle is blocked and the barn must
// move off it.  The statement's no-adjacency guarantee means this can only
// happen for a 1x1 rectangle (a 1x2 or 2x2 block would need adjacent cows), and
// then all four edge neighbours are free.  Rather than rely on that, the
// fallback searches outward generally: ring r holds every cell lying dx steps
// beyond the rectangle in x and dy beyond in y with dx+dy = r.  Since g and h
// are convex with minima on the rectangle, the cheapest cell of ring r is
//   min over dx+dy=r of gd(dx) + hd(dy),   gd(d) = min(g(xlo-d), g(xhi+d)),
// which is non-decreasing in r, so once it exceeds the best free cost found the
// search can stop -- no later ring can beat or tie it.  This also settles the
// count: ties can only live in rings still under that bound.
//
// Everything fits in 32-bit ints: cost <= 10000*40000 = 4e8 and the largest
// possible rectangle 20001x20001 = 4.0004e8, both below 2^31-1.  That avoids
// long long, which POJ's ancient compilers print inconsistently.

#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

static int n;
static vector<int> xs, ys;   // sorted coordinate multisets
static vector<int> px, py;   // prefix sums of the above
static vector<pair<int,int> > pts;  // distinct grazing spots, sorted

// sum of |v - a[i]| over the sorted multiset a
static int cost1(const vector<int>& a, const vector<int>& pre, int v) {
    int m = (int)a.size();
    int k = (int)(lower_bound(a.begin(), a.end(), v) - a.begin());
    return (k * v - pre[k]) + ((pre[m] - pre[k]) - (m - k) * v);
}

static bool occupied(int x, int y) {
    return binary_search(pts.begin(), pts.end(), make_pair(x, y));
}

static int bmin, bcnt;

static void consider(int x, int y) {
    if (occupied(x, y)) return;
    int c = cost1(xs, px, x) + cost1(ys, py, y);
    if (bmin < 0 || c < bmin) { bmin = c; bcnt = 1; }
    else if (c == bmin) bcnt++;
}

int main() {
    if (scanf("%d", &n) != 1) return 0;
    xs.resize(n); ys.resize(n); pts.resize(n);
    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        xs[i] = a; ys[i] = b;
        pts[i] = make_pair(a, b);
    }
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    px.assign(n + 1, 0); py.assign(n + 1, 0);
    for (int i = 0; i < n; i++) { px[i+1] = px[i] + xs[i]; py[i+1] = py[i] + ys[i]; }

    int xlo = xs[(n-1)/2], xhi = xs[n/2];
    int ylo = ys[(n-1)/2], yhi = ys[n/2];

    int gmin = cost1(xs, px, xlo), hmin = cost1(ys, py, ylo);
    int area = (xhi - xlo + 1) * (yhi - ylo + 1);
    int occ = 0;
    for (size_t i = 0; i < pts.size(); i++)
        if (pts[i].first >= xlo && pts[i].first <= xhi &&
            pts[i].second >= ylo && pts[i].second <= yhi) occ++;

    if (area - occ > 0) { printf("%d %d\n", gmin + hmin, area - occ); return 0; }

    bmin = -1; bcnt = 0;
    for (int r = 1; r <= 100000; r++) {
        int low = -1;
        for (int dx = 0; dx <= r; dx++) {
            int dy = r - dx;
            int gv = (dx == 0) ? gmin
                   : min(cost1(xs, px, xlo - dx), cost1(xs, px, xhi + dx));
            int hv = (dy == 0) ? hmin
                   : min(cost1(ys, py, ylo - dy), cost1(ys, py, yhi + dy));
            if (low < 0 || gv + hv < low) low = gv + hv;
        }
        if (bmin >= 0 && low > bmin) break;

        for (int dx = 0; dx <= r; dx++) {
            int dy = r - dx;
            if (dx == 0) {
                for (int x = xlo; x <= xhi; x++) {
                    consider(x, ylo - dy);
                    consider(x, yhi + dy);
                }
            } else if (dy == 0) {
                for (int y = ylo; y <= yhi; y++) {
                    consider(xlo - dx, y);
                    consider(xhi + dx, y);
                }
            } else {
                consider(xlo - dx, ylo - dy); consider(xlo - dx, yhi + dy);
                consider(xhi + dx, ylo - dy); consider(xhi + dx, yhi + dy);
            }
        }
    }
    printf("%d %d\n", bmin, bcnt);
    return 0;
}
