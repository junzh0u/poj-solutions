// POJ 2398 - Toy Storage
//
// Same box-and-partitions geometry as 2318 (TOYS): a rectangular box with
// (x1,y1) upper-left and (x2,y2) lower-right is split by n non-intersecting
// cardboard partitions, the i-th running from (Ui,y1) down to (Li,y2), and m
// toys are thrown in.  Two things differ from 2318:
//
//  - The partitions are NOT given left to right here.  Because they never
//    intersect, sorting them by Ui puts them in true left-to-right order (and
//    sorts the Li in the same pass), which is exactly the precondition the
//    binary search needs.  Sorting first is the whole difference in the
//    algorithm.
//  - The output is a histogram of bin occupancy, not per-bin counts: for each
//    t > 0 that occurs, how many partitions hold exactly t toys.  Empty bins
//    contribute nothing and are omitted, and each case is preceded by a line
//    reading "Box".
//
// With the partitions sorted, "the toy lies strictly left of partition i" is
// monotone in i - false for the partitions it has already passed, true from its
// own bin onward - so the bin index is the first i where the predicate holds,
// found by binary search.  O(n log n + m log n) per case.
//
// The side test is a cross product kept in exact integer arithmetic (long long):
// for the partition vector (Li-Ui, y2-y1) and the vector to the toy (X-Ui, Y-y1),
//
//     cross = (Li-Ui)*(Y-y1) - (y2-y1)*(X-Ui)
//
// is negative left of the partition and positive right of it, given the
// statement's orientation y1 > y2.  Floating point would buy nothing and only
// risks ties on near-collinear points, so none is used.  The `s` factor flips
// the comparison if a case were ever to hand over the two corners in the
// opposite vertical order.
//
// Notes on the statement:
//  - "You may assume that no toy will land on a cardboard", so cross == 0 never
//    occurs on real data and the tie-breaking in the search is arbitrary.  Toys
//    on the box boundary itself are legal and count as inside.
//  - x1 and x2 are read and never used: only the vertical extent enters the
//    cross product, and the partitions' own order defines left-to-right.
//  - Input ends at a line holding a single 0, which is the n field of a
//    would-be case.
//  - t ranges over 1..m, so the histogram array is indexed by toy count, not by
//    bin index.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

static const int MAXN = 1005;

struct Part { long long u, l; };
static Part P[MAXN];
static int cnt[MAXN];
static int hist[MAXN];

static bool byU(const Part &a, const Part &b) {
    if (a.u != b.u) return a.u < b.u;
    return a.l < b.l;
}

static int readInt(int &out) {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return 0;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    long long v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    out = (int)(sign * v);
    return 1;
}

int main() {
    int n, m, x1, y1, x2, y2;
    while (readInt(n)) {
        if (n == 0) break;
        if (!readInt(m)) break;
        readInt(x1); readInt(y1); readInt(x2); readInt(y2);
        (void)x1; (void)x2;
        long long dy = (long long)y2 - (long long)y1;
        long long s = (dy < 0) ? 1 : -1;
        int i;
        for (i = 0; i < n; ++i) {
            int u, l;
            readInt(u); readInt(l);
            P[i].u = u; P[i].l = l;
        }
        std::sort(P, P + n, byU);
        for (i = 0; i <= n; ++i) cnt[i] = 0;
        for (int j = 0; j < m; ++j) {
            int px, py;
            readInt(px); readInt(py);
            int lo = 0, hi = n;
            while (lo < hi) {
                int mid = (lo + hi) / 2;
                long long cross = ((P[mid].l - P[mid].u) * ((long long)py - (long long)y1)
                                 - dy * ((long long)px - P[mid].u)) * s;
                if (cross < 0) hi = mid; else lo = mid + 1;
            }
            ++cnt[lo];
        }
        for (i = 0; i <= m; ++i) hist[i] = 0;
        for (i = 0; i <= n; ++i) if (cnt[i] > 0) ++hist[cnt[i]];
        printf("Box\n");
        for (i = 1; i <= m; ++i) if (hist[i] > 0) printf("%d: %d\n", i, hist[i]);
    }
    return 0;
}
