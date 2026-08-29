// POJ 1259 - The Picnic
// Model: claude-sonnet-5
// Approach: largest empty convex polygon (Chvatal-Klincsek style DP),
// following the classic "enumerate bottom-left vertex" construction.
// Points are sorted by (y,x); for each index idx (as the anchor s, forced
// to be the lexicographically-smallest, hence bottommost-then-leftmost,
// vertex of the polygon), only later points in that order are candidates.
// They are sorted by polar angle around s (all lie within a half-open
// 180-degree wedge, so a cross-product comparator gives a consistent
// order); ties (points collinear with s) are broken by taking the farther
// point first. f(i,j) = max total (2x, via the fan-from-s decomposition)
// area of a convex, empty chain s -> ... -> q[i] -> q[j], memoized.
// Validity requires no candidate strictly between i and j (by sorted
// index) to lie strictly inside triangle(s,q[i],q[j]) - points exactly on
// an edge are allowed, per the problem's discuss board ("boundary points
// are fine, only strictly enclosed obstacles are forbidden"). Extending
// the chain backward into q[i] (giving it a real predecessor, turning the
// segment s-q[i] into an internal diagonal rather than a genuine polygon
// edge) is forbidden whenever q[i] has a nearer same-ray sibling q[i+1]
// (cross(s,q[i],q[i+1])==0): such a sibling would sit exactly on that
// diagonal and be wrongly treated as a safe boundary point by the local
// triangle check, even though it is genuinely enclosed once the diagonal
// stops being a real edge. This is a documented pitfall on the problem's
// discuss board (message 350624): "this line [ray from the anchor] can
// only be a starting edge", confirmed by reproducing the exact failure
// (the official sample area came out as 139.0 instead of 129.0 without
// this restriction, traced to a point sitting on the ray from the anchor
// through a middle vertex). The closing turn back to the anchor needs no
// separate check: cross3(A,B,C) is cyclic-invariant, so it equals the
// very same value already required to be non-negative for the pair
// itself. Verified against brute force (exhaustive subset enumeration)
// on thousands of random and dense-grid (heavy collinearity) cases, and
// against a documented failing case from the discuss board.
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long ll;

struct Pt { int x, y; };

static int m;
static Pt allpt[105];
static Pt s; // current anchor
static Pt q[105];
static int k;

static inline ll cross3(const Pt&O, const Pt&A, const Pt&B) {
    return (ll)(A.x - O.x) * (B.y - O.y) - (ll)(A.y - O.y) * (B.x - O.x);
}

static bool cmpHorizontal(const Pt&a, const Pt&b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

static bool cmpAngle(const Pt&a, const Pt&b) {
    ll cr = cross3(s, a, b);
    if (cr > 0) return true;
    if (cr < 0) return false;
    ll da = (ll)(a.x - s.x) * (a.x - s.x) + (ll)(a.y - s.y) * (a.y - s.y);
    ll db = (ll)(b.x - s.x) * (b.x - s.x) + (ll)(b.y - s.y) * (b.y - s.y);
    return da > db; // farther first on a tied angle
}

static ll rec[105][105];
static bool done_[105][105];

static ll f(int i, int j) {
    if (done_[i][j]) return rec[i][j];
    done_[i][j] = true;
    for (int kk = i + 1; kk < j; kk++) {
        ll c1 = cross3(s, q[i], q[kk]);
        ll c2 = cross3(q[i], q[j], q[kk]);
        ll c3 = cross3(q[j], s, q[kk]);
        if (c1 > 0 && c2 > 0 && c3 > 0) {
            rec[i][j] = -1;
            return rec[i][j];
        }
    }
    ll cij = cross3(s, q[i], q[j]);
    ll val = cij;
    if (val < 0) { rec[i][j] = -1; return rec[i][j]; }
    bool allowExtend;
    if (cij == 0) {
        allowExtend = true;
    } else {
        ll nextcross = (i + 1 < k) ? cross3(s, q[i], q[i + 1]) : 1;
        allowExtend = (nextcross != 0);
    }
    if (allowExtend) {
        ll best = -1;
        for (int kk = 0; kk < i; kk++) {
            ll t = cross3(q[kk], q[i], q[j]);
            if (t >= 0) {
                ll fv = f(kk, i);
                if (fv >= 0 && fv > best) best = fv;
            }
        }
        if (best >= 0) val += best;
    }
    rec[i][j] = val;
    return val;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &m);
        for (int i = 0; i < m; i++) scanf("%d %d", &allpt[i].x, &allpt[i].y);
        sort(allpt, allpt + m, cmpHorizontal);
        ll best = 0;
        for (int idx = 0; idx < m - 2; idx++) {
            s = allpt[idx];
            k = m - 1 - idx;
            for (int t = 0; t < k; t++) q[t] = allpt[t + idx + 1];
            sort(q, q + k, cmpAngle);
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++)
                    done_[i][j] = false;
            for (int i = 0; i < k - 1; i++)
                for (int j = i + 1; j < k; j++) {
                    ll v = f(i, j);
                    if (v > best) best = v;
                }
        }
        printf("%.1f\n", best / 2.0);
    }
    return 0;
}
