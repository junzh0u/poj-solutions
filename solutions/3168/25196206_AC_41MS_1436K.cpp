// POJ 3168 - Barn Expansion
// Model: claude-sonnet-5
//
// Approach: barns are pairwise interior-disjoint axis-aligned rectangles.
// A barn cannot expand iff its closed rectangle intersects the closed
// rectangle of some other barn (since interiors never overlap, any such
// intersection is exactly a shared corner or a shared piece of wall, which
// is precisely the "share a corner or a wall" condition in the statement).
//
// That intersection can only happen in two shapes:
//  1. A vertical shared segment: barn A's right x equals barn B's left x
//     (or vice versa), and their y-intervals [B,D] intersect (as closed
//     intervals, so touching at a single point counts too).
//  2. A horizontal shared segment: barn A's top y equals barn B's bottom y
//     (or vice versa), and their x-intervals [A,C] intersect similarly.
// A pure corner touch (single point) is the degenerate case of intervals
// that intersect at exactly one point, and is caught by case 1 (and
// redundantly by case 2) automatically -- so no separate corner handling
// is needed, matching a board post that solves it the same way, treating a
// shared corner as a zero-length shared edge.
//
// For each distinct edge coordinate x0, group barns whose right edge is x0
// (call R) and barns whose left edge is x0 (call L). Because barns don't
// overlap, the y-intervals within R alone are pairwise non-overlapping
// (may only touch at a point), and likewise within L. So R and L are each
// individually sorted, non-overlapping interval sets, and finding every
// intersecting R-L pair is the classic two-pointer "interval list
// intersection" merge, linear in |R|+|L|. Summed over all x0 this is O(N)
// (each barn contributes exactly one entry to some R-group and one entry
// to some L-group), on top of an O(N log N) sort. The horizontal case is
// symmetric with x/y swapped.
//
// Board post 163667 warns of a WA from a missing "=" -- i.e. using strict
// interval intersection instead of closed/touching -- consistent with the
// closed-interval (<=) test used here. Board post 138950 independently
// describes exactly this edge-sweep approach with corners as zero-length
// edges, and 148781 warns hand-rolled quicksort has adversarial killer
// data (a "1..x..1" bitonic sequence); std::sort (introsort) is immune to
// that pathological case, so it is used throughout instead of a manual
// quicksort.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 25005;
int A[MAXN], B[MAXN], C[MAXN], D[MAXN];
bool touched[MAXN];

struct Ev {
    int x;
    int type; // 0 = "end" edge (R group), 1 = "start" edge (L group)
    int idx;
};

bool evCmp(const Ev &a, const Ev &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.type < b.type;
}

struct CmpByKey {
    const int *key;
    CmpByKey(const int *k) : key(k) {}
    bool operator()(int a, int b) const { return key[a] < key[b]; }
};

// edgeEnd[i]/edgeStart[i]: the coordinate defining the "R"/"L" groups.
// ilo[i]/ihi[i]: the interval bounds to test for intersection.
void processCategory(const int *edgeEnd, const int *edgeStart,
                      const int *ilo, const int *ihi, int n) {
    vector<Ev> ev(2 * n);
    for (int i = 0; i < n; i++) {
        ev[2 * i].x = edgeEnd[i];
        ev[2 * i].type = 0;
        ev[2 * i].idx = i;
        ev[2 * i + 1].x = edgeStart[i];
        ev[2 * i + 1].type = 1;
        ev[2 * i + 1].idx = i;
    }
    sort(ev.begin(), ev.end(), evCmp);

    CmpByKey cmp(ilo);
    int m = (int)ev.size();
    int i = 0;
    vector<int> R, L;
    while (i < m) {
        int j = i;
        while (j < m && ev[j].x == ev[i].x) j++;
        R.clear();
        L.clear();
        for (int k = i; k < j; k++) {
            if (ev[k].type == 0) R.push_back(ev[k].idx);
            else L.push_back(ev[k].idx);
        }
        if (!R.empty() && !L.empty()) {
            sort(R.begin(), R.end(), cmp);
            sort(L.begin(), L.end(), cmp);
            size_t p = 0, q = 0;
            while (p < R.size() && q < L.size()) {
                int ri = R[p], li = L[q];
                int lo = max(ilo[ri], ilo[li]);
                int hi = min(ihi[ri], ihi[li]);
                if (lo <= hi) {
                    touched[ri] = true;
                    touched[li] = true;
                }
                if (ihi[ri] < ihi[li]) p++;
                else if (ihi[li] < ihi[ri]) q++;
                else { p++; q++; }
            }
        }
        i = j;
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &A[i], &B[i], &C[i], &D[i]);
        touched[i] = false;
    }

    processCategory(C, A, B, D, n); // vertical shared edges (shared x)
    processCategory(D, B, A, C, n); // horizontal shared edges (shared y)

    int count = 0;
    for (int i = 0; i < n; i++) if (!touched[i]) count++;
    printf("%d\n", count);
    return 0;
}
