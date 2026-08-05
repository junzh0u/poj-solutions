// POJ 2482 - Stars in Your Window
//
// Model: claude-sonnet-5
//
// A W x H axis-aligned window (edges excluded) is translated over the plane
// to maximize the sum of brightness of stars strictly inside it. A star at
// (x,y) is inside a window whose lower-left corner is (a,b) iff
// a in (x-W, x) and b in (y-H, y). So each star contributes to a 2D open
// rectangle of valid corner positions; we need the point covered by the
// most weight.
//
// Sweep the corner's x-coordinate `a` over the critical values {x-W, x} for
// every star, maintaining (via a segment tree over compressed y-coordinates)
// the sum of weights of stars currently horizontally active, indexed by b.
// At x-W a star's y-interval (y-H, y) gets +c; at x it gets -c. Because both
// horizontal and vertical membership are open intervals, and add/remove
// events only add non-negative weight, the true global maximum is always
// attained in the open interval strictly after some critical x (all ties at
// the same x processed together, then query) -- a state at the critical
// point itself is always <= the fully-updated state just after it.
//
// The y-axis is compressed using all 2n values {y-H, y}; a star's interval
// (y-H, y) maps to compressed elementary sub-interval indices [lo, hi-1]
// (hi-1 >= lo always since H >= 1 keeps y-H < y, both present as distinct
// breakpoints). Segment tree supports range-add / global-max.
//
// Coordinates: 0 <= x,y < 2^31, so stored as long long to keep x-W etc.
// signed-safe; brightness sums fit easily in long long too.
//
// Complexity: O(n log n) per test case.

#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

const int MAXN = 10005;

int n;
ll W, H;
ll starX[MAXN], starY[MAXN];
int starC[MAXN];

ll ycoord[2 * MAXN];

struct XEvent {
    ll pos;
    int lo, hi;
    ll val;
};
XEvent xev[2 * MAXN];

bool cmpXEvent(const XEvent &a, const XEvent &b) {
    return a.pos < b.pos;
}

ll segMax[4 * 2 * MAXN];
ll segLazy[4 * 2 * MAXN];

void build(int node, int l, int r) {
    segMax[node] = 0;
    segLazy[node] = 0;
    if (l == r) return;
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
}

void update(int node, int l, int r, int ql, int qr, ll val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        segMax[node] += val;
        segLazy[node] += val;
        return;
    }
    int mid = (l + r) / 2;
    update(node * 2, l, mid, ql, qr, val);
    update(node * 2 + 1, mid + 1, r, ql, qr, val);
    segMax[node] = max(segMax[node * 2], segMax[node * 2 + 1]) + segLazy[node];
}

int main() {
    while (scanf("%d %lld %lld", &n, &W, &H) == 3) {
        for (int i = 0; i < n; i++) {
            ll x, y;
            int c;
            scanf("%lld %lld %d", &x, &y, &c);
            starX[i] = x;
            starY[i] = y;
            starC[i] = c;
        }

        int ycnt = 0;
        for (int i = 0; i < n; i++) {
            ycoord[ycnt++] = starY[i] - H;
            ycoord[ycnt++] = starY[i];
        }
        sort(ycoord, ycoord + ycnt);
        int m = (int)(unique(ycoord, ycoord + ycnt) - ycoord);

        int ecnt = 0;
        for (int i = 0; i < n; i++) {
            int lo = (int)(lower_bound(ycoord, ycoord + m, starY[i] - H) - ycoord);
            int hi = (int)(lower_bound(ycoord, ycoord + m, starY[i]) - ycoord);
            xev[ecnt].pos = starX[i] - W;
            xev[ecnt].lo = lo;
            xev[ecnt].hi = hi - 1;
            xev[ecnt].val = starC[i];
            ecnt++;
            xev[ecnt].pos = starX[i];
            xev[ecnt].lo = lo;
            xev[ecnt].hi = hi - 1;
            xev[ecnt].val = -starC[i];
            ecnt++;
        }
        sort(xev, xev + ecnt, cmpXEvent);

        int segN = m - 1;
        if (segN < 1) segN = 1;
        build(1, 0, segN - 1);

        ll ans = 0;
        int i = 0;
        while (i < ecnt) {
            int j = i;
            while (j < ecnt && xev[j].pos == xev[i].pos) {
                update(1, 0, segN - 1, xev[j].lo, xev[j].hi, xev[j].val);
                j++;
            }
            if (segMax[1] > ans) ans = segMax[1];
            i = j;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
