// POJ 2066 - Minimax Triangulation
// Model: claude-sonnet-5
// Approach: Interval DP over the polygon's vertex chain. dp[i][j] is the min possible
// "largest triangle area" when triangulating the sub-polygon bounded by the boundary
// chain v_i..v_j plus the chord (v_i, v_j) (assumed already established, either as a
// real polygon edge when j=i+1, or as a validated diagonal). For i<j and a split point
// k in (i,j): dp[i][j] = min over valid k of max(dp[i][k], dp[k][j], area(i,k,j)),
// where a split is valid only if chord (i,k) and (k,j) are each either polygon edges
// (k adjacent) or validated diagonals. The final answer is dp[0][n-1], since (n-1,0)
// is the real closing polygon edge.
//
// Diagonal validity g[i][j] (polygon may be non-convex, only guaranteed simple) is
// checked by: (1) no other vertex lies exactly on the open segment (i,j); (2) the
// segment does not intersect (cross or touch) any polygon edge not incident to i or j;
// (3) the segment's midpoint is strictly inside the polygon (winding-number test).
// For a simple polygon, (2)+(3) together imply the whole open segment lies in the
// interior, since inside/outside status can only change by crossing the boundary.
// All arithmetic is exact integers: areas are tracked as twice-area, and the midpoint
// test point is x[i]+x[j], y[i]+y[j] (= 2*midpoint) checked against the polygon scaled
// by 2, avoiding any fractional coordinates.
//
// Statement is unambiguous on framing: n scenarios, each with an exact vertex count m
// (3<=m<=49) followed by exactly m vertex lines; no trailing/blank-line surprises found
// on the discuss board for this problem.

#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 55;
int n;
ll x[MAXN], y[MAXN];
ll Xs[MAXN], Ys[MAXN];
bool g[MAXN][MAXN];
ll dp[MAXN][MAXN];
const ll INF = (ll)4e18;

ll cross(ll ox, ll oy, ll ax, ll ay, ll bx, ll by) {
    return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

bool onSegment(ll px, ll py, ll ax, ll ay, ll bx, ll by) {
    return min(ax, bx) <= px && px <= max(ax, bx) &&
           min(ay, by) <= py && py <= max(ay, by);
}

bool segIntersect(ll ax, ll ay, ll bx, ll by, ll cx, ll cy, ll dx, ll dy) {
    ll d1 = cross(cx, cy, dx, dy, ax, ay);
    ll d2 = cross(cx, cy, dx, dy, bx, by);
    ll d3 = cross(ax, ay, bx, by, cx, cy);
    ll d4 = cross(ax, ay, bx, by, dx, dy);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
    if (d1 == 0 && onSegment(ax, ay, cx, cy, dx, dy)) return true;
    if (d2 == 0 && onSegment(bx, by, cx, cy, dx, dy)) return true;
    if (d3 == 0 && onSegment(cx, cy, ax, ay, bx, by)) return true;
    if (d4 == 0 && onSegment(dx, dy, ax, ay, bx, by)) return true;
    return false;
}

ll isLeft(ll ox, ll oy, ll ax, ll ay, ll px, ll py) {
    return (ax - ox) * (py - oy) - (px - ox) * (ay - oy);
}

bool pointInPoly(ll px, ll py) {
    int wn = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        ll yi = Ys[i], yj = Ys[j];
        if (yi <= py) {
            if (yj > py) {
                if (isLeft(Xs[i], Ys[i], Xs[j], Ys[j], px, py) > 0) wn++;
            }
        } else {
            if (yj <= py) {
                if (isLeft(Xs[i], Ys[i], Xs[j], Ys[j], px, py) < 0) wn--;
            }
        }
    }
    return wn != 0;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%lld %lld", &x[i], &y[i]);
            Xs[i] = 2 * x[i];
            Ys[i] = 2 * y[i];
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                g[i][j] = false;

        for (int i = 0; i < n; i++) {
            for (int j = i + 2; j < n; j++) {
                bool valid = true;
                for (int p = 0; p < n && valid; p++) {
                    if (p == i || p == j) continue;
                    if (cross(x[i], y[i], x[j], y[j], x[p], y[p]) == 0 &&
                        onSegment(x[p], y[p], x[i], y[i], x[j], y[j])) {
                        valid = false;
                    }
                }
                for (int k = 0; k < n && valid; k++) {
                    int k2 = (k + 1) % n;
                    if (k == i || k == j || k2 == i || k2 == j) continue;
                    if (segIntersect(x[i], y[i], x[j], y[j], x[k], y[k], x[k2], y[k2]))
                        valid = false;
                }
                if (valid) {
                    ll tx = x[i] + x[j], ty = y[i] + y[j];
                    if (!pointInPoly(tx, ty)) valid = false;
                }
                g[i][j] = g[j][i] = valid;
            }
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                dp[i][j] = INF;
        for (int i = 0; i + 1 < n; i++) dp[i][i + 1] = 0;

        for (int gap = 2; gap < n; gap++) {
            for (int i = 0; i + gap < n; i++) {
                int j = i + gap;
                ll best = INF;
                for (int k = i + 1; k < j; k++) {
                    bool leftOK = (k == i + 1) || g[i][k];
                    bool rightOK = (k == j - 1) || g[k][j];
                    if (!leftOK || !rightOK) continue;
                    if (dp[i][k] == INF || dp[k][j] == INF) continue;
                    ll area2 = cross(x[i], y[i], x[k], y[k], x[j], y[j]);
                    if (area2 < 0) area2 = -area2;
                    ll val = max(dp[i][k], max(dp[k][j], area2));
                    if (val < best) best = val;
                }
                dp[i][j] = best;
            }
        }

        ll ans2 = dp[0][n - 1];
        printf("%.1f\n", ans2 / 2.0);
    }
    return 0;
}
