// POJ 2944 - Wild West
// Model: claude-sonnet-5
//
// A hero with scores (x,y,z) fails to beat an evil member (a,b,c) iff
// x<=a, y<=b, z<=c (weak domination in all three skills, since "defeat"
// requires being strictly higher in at least one skill).  So the set of
// non-heroes is the union, over all evil members, of the axis-aligned box
// [1,a]x[1,b]x[1,c].  (A member of the evil society is itself inside its
// own box, so "the Hero cannot be a member of the evil society" is implied
// automatically and needs no separate handling.)
//
// Answer = m^3 - volume(union of these origin-anchored boxes).
//
// Only the Pareto-maximal (a,b,c) points matter: a dominated evil member's
// box is a subset of the dominating member's box.  Sweep the first
// coordinate a from m down to 1, maintaining, for the members with a-value
// already swept in (i.e. a-value >= current x), the 2D staircase (skyline)
// of maximal (b,c) pairs together with the union area of the anchored
// rectangles [1,b]x[1,c] over that skyline.  The area is piecewise constant
// between distinct a-values, so we only need to touch the sweep at the n
// distinct a-values of the points (grouped, processed in descending order),
// each contributing area_i * (a_i - a_{i+1}) to the total swept volume.
//
// The skyline is kept in a std::map<int,int> keyed by b (ascending) with c
// strictly decreasing along it.  Inserting a new (B,C): if it is dominated
// by the point with the smallest b>=B, skip; otherwise remove every skyline
// point dominated by (B,C) (all have b<=B, c<=C, forming a contiguous run
// just left of B) and splice the new point in, maintaining a running total
// area = sum (b_i - b_{i-1}) * c_i incrementally.  Each point is inserted
// and removed from the skyline at most once overall, so the whole sweep for
// one test case is O(n log n), independent of m.
//
// Values fit in a 64-bit integer: m <= 1e5 so m^3 <= 1e15.
//
// Verified by hand against all three sample cases (848, 19, 999999999992)
// and against brute-force inclusion-exclusion / direct O(m^3) enumeration
// on small random cases.

#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

struct Point {
    int a, b, c;
};

bool cmpDesc(const Point &x, const Point &y) {
    return x.a > y.a;
}

static void skylineInsert(map<int, int> &sky, long long &area, int B, int C) {
    map<int, int>::iterator it2 = sky.lower_bound(B);
    if (it2 != sky.end() && it2->second >= C) {
        return; // dominated by an existing point, no change
    }

    bool exactMatch = (it2 != sky.end() && it2->first == B);

    // Identify R (the surviving right neighbor once the exact-match node, if
    // any, is removed) and R's predecessor key as it stood BEFORE any
    // erasure -- that is what the running 'area' currently reflects, so
    // that is what must be subtracted before we start mutating the map.
    map<int, int>::iterator R_it;
    bool hasR;
    long long rOrigPredB = 0;
    if (exactMatch) {
        R_it = it2;
        ++R_it;
        hasR = (R_it != sky.end());
        if (hasR) {
            rOrigPredB = it2->first; // the exact-match node (key == B) was R's predecessor
        }
    } else {
        R_it = it2;
        hasR = (R_it != sky.end());
        if (hasR && R_it != sky.begin()) {
            map<int, int>::iterator p = R_it;
            --p;
            rOrigPredB = p->first;
        }
    }
    if (hasR) {
        area -= (long long)(R_it->first - rOrigPredB) * R_it->second;
    }

    if (exactMatch) {
        long long predB = 0;
        if (it2 != sky.begin()) {
            map<int, int>::iterator pred = it2;
            --pred;
            predB = pred->first;
        }
        area -= (long long)(it2->first - predB) * it2->second;
        sky.erase(it2);
        it2 = R_it;
    }

    while (it2 != sky.begin()) {
        map<int, int>::iterator pit = it2;
        --pit;
        if (pit->second <= C) {
            long long predB2 = 0;
            if (pit != sky.begin()) {
                map<int, int>::iterator pp = pit;
                --pp;
                predB2 = pp->first;
            }
            area -= (long long)(pit->first - predB2) * pit->second;
            sky.erase(pit);
        } else {
            break;
        }
    }

    long long leftB = 0;
    if (it2 != sky.begin()) {
        map<int, int>::iterator L = it2;
        --L;
        leftB = L->first;
    }

    area += (long long)(B - leftB) * C;

    if (hasR) {
        area += (long long)(it2->first - B) * it2->second;
    }

    sky[B] = C;
}

int main() {
    int n;
    long long m;
    static Point pts[100000];
    while (scanf("%d %lld", &n, &m) == 2 && (n || m)) {
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d", &pts[i].a, &pts[i].b, &pts[i].c);
        }
        sort(pts, pts + n, cmpDesc);

        map<int, int> sky;
        long long area = 0;
        long long totalUnion = 0;

        int i = 0;
        while (i < n) {
            int j = i;
            int curA = pts[i].a;
            while (j < n && pts[j].a == curA) {
                skylineInsert(sky, area, pts[j].b, pts[j].c);
                j++;
            }
            long long nextA = (j < n) ? pts[j].a : 0;
            totalUnion += area * (curA - nextA);
            i = j;
        }

        long long ans = m * m * m - totalUnion;
        printf("%lld\n", ans);
    }
    return 0;
}
