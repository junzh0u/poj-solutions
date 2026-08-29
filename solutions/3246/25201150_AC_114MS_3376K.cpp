// POJ 3246 - Game
// Model: claude-sonnet-5
//
// After removing one convex-hull vertex V_i, every other old hull vertex
// stays on the new hull (a hull vertex of a point set remains a hull vertex
// of any subset that still contains it). Only the boundary near V_i
// changes: the two edges (V_{i-1},V_i) and (V_i,V_{i+1}) are replaced by the
// "outer" convex chain (relative to chord V_{i-1}V_{i+1}) of whichever
// points still stick out beyond that chord on V_i's side. Any such point
// must lie inside the "ear" triangle (V_{i-1},V_i,V_{i+1}).
//
// A single point can matter for more than one ear (adjacent ears' triangles
// can geometrically overlap), but every point inside ear i has an
// x-coordinate between the x-coordinates of the two hull edges touching
// V_i, and those two edges both belong to either the upper or the lower
// x-monotone chain of the hull. So: binary-search each point's x on the
// upper chain and on the lower chain to find the (at most two) hull edges
// whose x-range contains it, then test the point against the (at most four)
// ears touching those edges. This gives O(1) amortized/O(log h) buckets per
// point and O(N) total (point, ear) incidences overall.
//
// For each ear with candidates, rebuild the exact convex hull of
// {V_{i-1}} u candidates u {V_{i+1}} with the same robust monotone-chain
// routine (this handles points collinear with an old edge, or with each
// other, correctly instead of a hand-rolled projection sort), then extract
// the arc that bulges towards V_i (the arc with more than the two
// endpoints; the other arc is the bare chord). The new area for that
// removal is old_area - triangle(V_{i-1},V_i,V_{i+1}) + area(that arc's cap
// against the chord). The minimum over all ears (an ear with no candidates
// trivially contributes old_area - triangle) is the answer.
//
// All areas are tracked as exact integers (2*area) using __int128, so the
// final answer (always a multiple of 0.5 for integer input) is printed
// exactly with no floating point involved at all.
//
// Statement/board note: coordinates are said to be within 1e9 in absolute
// value on the discuss board (not stated in the problem body itself);
// __int128 is used throughout so this isn't even a close call.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef __int128 lll;

struct Pt {
    ll x, y;
};

static inline lll cross128(const Pt& O, const Pt& A, const Pt& B) {
    return (lll)(A.x - O.x) * (lll)(B.y - O.y) - (lll)(A.y - O.y) * (lll)(B.x - O.x);
}
static inline lll crossRaw(const Pt& A, const Pt& B) {
    return (lll)A.x * (lll)B.y - (lll)A.y * (lll)B.x;
}

struct PtLess {
    bool operator()(const Pt& a, const Pt& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};
struct PtEq {
    bool operator()(const Pt& a, const Pt& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

// Standard Andrew's monotone chain: returns the CCW hull of pts (a plain
// point list, not required to be sorted or deduplicated beforehand).
vector<Pt> buildHull(vector<Pt> p) {
    sort(p.begin(), p.end(), PtLess());
    p.erase(unique(p.begin(), p.end(), PtEq()), p.end());
    int m = (int)p.size();
    if (m < 3) return p;
    vector<Pt> lo, up;
    for (int i = 0; i < m; i++) {
        while (lo.size() >= 2 && cross128(lo[lo.size() - 2], lo[lo.size() - 1], p[i]) <= 0) lo.pop_back();
        lo.push_back(p[i]);
    }
    for (int i = m - 1; i >= 0; i--) {
        while (up.size() >= 2 && cross128(up[up.size() - 2], up[up.size() - 1], p[i]) <= 0) up.pop_back();
        up.push_back(p[i]);
    }
    if (lo.size() < 2 || up.size() < 2) return p;
    lo.pop_back();
    up.pop_back();
    vector<Pt> H = lo;
    for (size_t i = 0; i < up.size(); i++) H.push_back(up[i]);
    return H;
}

int n;
vector<Pt> pts;

int main() {
    while (scanf("%d", &n) == 1 && n) {
        pts.assign(n, Pt());
        for (int i = 0; i < n; i++) scanf("%lld %lld", &pts[i].x, &pts[i].y);

        vector<Pt> s = pts;
        sort(s.begin(), s.end(), PtLess());
        int m = (int)s.size();

        vector<int> lo, up;
        for (int i = 0; i < m; i++) {
            while (lo.size() >= 2 && cross128(s[lo[lo.size() - 2]], s[lo[lo.size() - 1]], s[i]) <= 0) lo.pop_back();
            lo.push_back(i);
        }
        for (int i = m - 1; i >= 0; i--) {
            while (up.size() >= 2 && cross128(s[up[up.size() - 2]], s[up[up.size() - 1]], s[i]) <= 0) up.pop_back();
            up.push_back(i);
        }
        // lo: leftmost..rightmost ascending x (indices into s)
        // up: rightmost..leftmost descending x (indices into s)

        if ((int)lo.size() < 2 || (int)up.size() < 2) {
            printf("0.00\n");
            continue;
        }

        int L = (int)lo.size();
        int U = (int)up.size();
        vector<Pt> H;
        for (int k = 0; k < L - 1; k++) H.push_back(s[lo[k]]);
        for (int k = 0; k < U - 1; k++) H.push_back(s[up[k]]);
        int h = (int)H.size();

        if (h < 3) {
            printf("0.00\n");
            continue;
        }

        int splitLower = L - 1; // H[splitLower] is the rightmost point

        vector<int> lowerChainH; // H-indices, x ascending, leftmost..rightmost
        for (int k = 0; k <= splitLower; k++) lowerChainH.push_back(k);
        vector<int> upperChainH; // H-indices, x ascending, leftmost..rightmost
        upperChainH.push_back(0);
        for (int k = h - 1; k >= splitLower; k--) upperChainH.push_back(k);

        lll area0_2 = 0;
        for (int k = 0; k < h; k++) {
            int k2 = (k + 1 == h) ? 0 : k + 1;
            area0_2 += crossRaw(H[k], H[k2]);
        }
        if (area0_2 < 0) area0_2 = -area0_2; // H is CCW by construction; defensive only

        vector<Pt> hullSorted = H;
        sort(hullSorted.begin(), hullSorted.end(), PtLess());

        vector<vector<Pt> > bucket(h);

        for (int idx = 0; idx < n; idx++) {
            Pt P = pts[idx];
            vector<Pt>::iterator hit = lower_bound(hullSorted.begin(), hullSorted.end(), P, PtLess());
            if (hit != hullSorted.end() && hit->x == P.x && hit->y == P.y) continue; // hull point, skip

            for (int which = 0; which < 2; which++) {
                vector<int>& chain = (which == 0) ? lowerChainH : upperChainH;
                int sz = (int)chain.size();
                if (sz < 2) continue;

                int lo2 = 0, hi2 = sz;
                while (lo2 < hi2) {
                    int mid = (lo2 + hi2) / 2;
                    if (H[chain[mid]].x > P.x) hi2 = mid; else lo2 = mid + 1;
                }
                int k = lo2 - 1;
                if (k < 0) k = 0;
                if (k > sz - 2) k = sz - 2;
                int a = chain[k], b = chain[k + 1];

                int cand[2];
                cand[0] = a;
                cand[1] = b;
                for (int t = 0; t < 2; t++) {
                    int i = cand[t];
                    int ip = (i - 1 + h) % h;
                    int inx = (i + 1) % h;
                    const Pt& A = H[ip];
                    const Pt& Vi = H[i];
                    const Pt& B = H[inx];
                    lll sideVi = cross128(A, B, Vi);
                    if (sideVi == 0) continue;
                    lll sideP = cross128(A, B, P);
                    bool sameSign = (sideVi > 0 && sideP > 0) || (sideVi < 0 && sideP < 0);
                    if (sameSign) bucket[i].push_back(P);
                }
            }
        }

        lll best2 = 0;
        bool haveBest = false;

        for (int i = 0; i < h; i++) {
            int ip = (i - 1 + h) % h;
            int inx = (i + 1) % h;
            const Pt& A = H[ip];
            const Pt& Vi = H[i];
            const Pt& B = H[inx];

            lll localOld = crossRaw(A, Vi) + crossRaw(Vi, B);
            lll localNew;

            if (bucket[i].empty()) {
                localNew = crossRaw(A, B);
            } else {
                vector<Pt> pool = bucket[i];
                pool.push_back(A);
                pool.push_back(B);
                vector<Pt> H2 = buildHull(pool);
                int h2 = (int)H2.size();

                int idxA = -1, idxB = -1;
                for (int t = 0; t < h2; t++) {
                    if (H2[t].x == A.x && H2[t].y == A.y) idxA = t;
                    if (H2[t].x == B.x && H2[t].y == B.y) idxB = t;
                }

                if (idxA < 0 || idxB < 0 || h2 < 3) {
                    // degenerate (shouldn't happen since A,B are extreme in the
                    // original point set and remain extreme in any subset)
                    localNew = crossRaw(A, B);
                } else {
                    // Two arcs from idxA to idxB around H2; the "direct" one (no
                    // interior points) is a bare edge, the other bulges via the
                    // candidates. Pick whichever arc is longer than 2 points; if
                    // both are exactly 2 (no effective candidates survived, e.g.
                    // all dominated/collinear), fall back to the bare chord.
                    vector<Pt> fwd, bwd;
                    for (int t = idxA;; t = (t + 1) % h2) {
                        fwd.push_back(H2[t]);
                        if (t == idxB) break;
                    }
                    for (int t = idxA;; t = (t - 1 + h2) % h2) {
                        bwd.push_back(H2[t]);
                        if (t == idxB) break;
                    }
                    vector<Pt>* chosen = 0;
                    if (fwd.size() > 2 && bwd.size() == 2) chosen = &fwd;
                    else if (bwd.size() > 2 && fwd.size() == 2) chosen = &bwd;
                    else if (fwd.size() > 2 && bwd.size() > 2) {
                        // Shouldn't happen (candidates are all on one side of AB),
                        // but pick the one actually on V_i's side defensively.
                        lll sideVi = cross128(A, B, Vi);
                        lll sideFwd = (fwd.size() >= 3) ? cross128(A, B, fwd[1]) : 0;
                        bool fwdMatches = (sideVi > 0 && sideFwd > 0) || (sideVi < 0 && sideFwd < 0);
                        chosen = fwdMatches ? &fwd : &bwd;
                    } else {
                        chosen = &fwd; // both length 2: bare chord either way
                    }

                    // chosen currently runs from A to B in one of the two
                    // directions; crossRaw sum needs the SAME orientation as the
                    // rest of the shoelace computation (A -> ... -> B), so if it
                    // was built backward (idxA down to idxB), reverse it first.
                    vector<Pt> chain = *chosen;
                    if (chosen == &bwd) reverse(chain.begin(), chain.end());

                    localNew = 0;
                    for (size_t t = 0; t + 1 < chain.size(); t++) localNew += crossRaw(chain[t], chain[t + 1]);
                }
            }

            lll delta = localNew - localOld;
            lll newArea2 = area0_2 + delta;
            if (newArea2 < 0) newArea2 = -newArea2;

            if (!haveBest || newArea2 < best2) {
                best2 = newArea2;
                haveBest = true;
            }
        }

        ll intPart = (ll)(best2 / 2);
        bool half = ((best2 % 2) != 0);
        if (half) printf("%lld.50\n", intPart);
        else printf("%lld.00\n", intPart);
    }
    return 0;
}
