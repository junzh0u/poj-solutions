// POJ 1727 - Advanced Causal Measurements (ACM)
// Model: claude-sonnet-5
//
// Approach: An event e=(t,x) is caused by c=(t0,x0) iff t >= t0 + |x-x0|.
// Substituting u=t+x, v=t-x turns this into two independent linear
// constraints: u >= u0 and v >= v0. So a set (group) of events sharing one
// common cause admits the *latest possible* cause time exactly when the
// cause sits at (u0,v0) = (min u over group, min v over group); the group's
// achievable cause time is floor((minU+minV)/2). Because coordinates (and,
// per the discuss board / "not divisible" wording, the hypothetical causes
// too) live on an integer lattice, and since minU,minV are both integers,
// any real-feasible cause point can be replaced by an integer one with the
// same floor((minU+minV)/2) value (the interval of feasible x0 for a given
// integer t0 has integer endpoints, so it's never empty once non-empty).
//
// We must partition the n events into at most m groups to maximize the
// minimum group value. Binary search the answer T (range: T can be as low
// as -2,000,000, per board discussion of two extreme events forcing a very
// early common cause; T is bounded above by min t_i, since any group
// containing point i has value <= t_i).
//
// Feasibility check for candidate T: minimum number of groups needed so
// every group satisfies minU+minV >= 2T. Process points sorted by u
// ascending. A group's minU is fixed forever at creation (the u of its
// first, smallest-u member, since later members only have larger-or-equal
// u). One can show algebraically that a later point p can be added to an
// existing group g (with founding value u0=g.minU) at ANY time iff
// p.v >= 2T - u0 -- this condition does not depend on which/how many points
// already joined g, only on u0. Consequently only the group with the
// LARGEST u0 among those created so far ever matters: if the best (max) u0
// doesn't satisfy the incoming point's requirement, no smaller u0 does
// either. So the whole simulation collapses to tracking a single scalar
// "bestU" (max u0 among groups opened so far): for each point (u ascending),
// if bestU >= 2T - v, absorb into existing groups (no-op); otherwise open a
// new group (count++, bestU=u, valid since u is the new running maximum).
// This makes each feasibility check O(n) after one O(n log n) sort, and the
// whole binary search O(n log n + n log(range)).
//
// Verified against all 4 sample cases by hand (m=1..4 on the same 4 events
// gives -2, 0, 0, 1) including the m=3 case, which requires the integer-cause
// floor (a naive real-valued/unfloored formula gives 0.5, not 0).
//
// I/O: the discuss board (message 5528 and followups) reports the C++
// <iostream> stream was too slow on POJ's judge for this problem back when
// it was set (TLE with cin, ~1s with scanf), so this uses scanf/printf.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

struct Pt {
    int u, v;
};

int n, m;
Pt pts[MAXN];
int tvals[MAXN];

bool byU(const Pt &a, const Pt &b) { return a.u < b.u; }

// minimum number of groups needed for candidate T; early-exits (returns m+1)
// once it's clear more than m groups are required.
int minGroups(int T, int cap) {
    // all u,v,T values are within +-4,000,000, so plain int (>=32-bit) is
    // ample; no need for long long (which isn't standard C++98 anyway).
    int bestU = -10000000; // safely below any real threshold
    int groups = 0;
    for (int i = 0; i < n; ++i) {
        int threshold = 2 * T - pts[i].v;
        if (bestU < threshold) {
            ++groups;
            if (groups > cap) return groups; // early exit, infeasible
            bestU = pts[i].u;
        }
    }
    return groups;
}

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc = 1; tc <= cases; ++tc) {
        scanf("%d %d", &n, &m);
        int hi = 2000000; // will be reduced to min t_i
        for (int i = 0; i < n; ++i) {
            int t, x;
            scanf("%d %d", &t, &x);
            pts[i].u = t + x;
            pts[i].v = t - x;
            tvals[i] = t;
            if (t < hi) hi = t;
        }
        sort(pts, pts + n, byU);

        int lo = -2000001;
        // binary search largest T in [lo,hi] with minGroups(T) <= m
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2; // bias high, safe for negatives
            if (minGroups(mid, m) <= m) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        printf("Case %d: %d\n", tc, lo);
    }
    return 0;
}
