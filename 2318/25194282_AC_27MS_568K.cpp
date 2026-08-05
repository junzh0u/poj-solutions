// POJ 2318 - TOYS
//
// A rectangular box with (x1,y1) upper-left and (x2,y2) lower-right is split by
// n non-intersecting cardboard partitions, the i-th running from (Ui,y1) down to
// (Li,y2).  Count how many of the m thrown toys land in each of the n+1 bins.
//
// Approach: the partitions are given sorted left to right and never cross, so for
// a fixed toy the predicate "the toy lies strictly left of partition i" is monotone
// in i - false for the partitions it has already passed, true from its own bin
// onward.  That makes the bin index the first i where the predicate holds, found by
// binary search, so the whole thing is O((n + m log n)) per case.
//
// The side test is a cross product kept in exact integer arithmetic (long long):
// for the partition vector (Li-Ui, y2-y1) and the vector to the toy (X-Ui, Y-y1),
//
//     cross = (Li-Ui)*(Y-y1) - (y2-y1)*(X-Ui)
//
// is negative left of the partition and positive right of it, given the statement's
// orientation y1 > y2.  Floating point would buy nothing here and only risks ties on
// near-collinear points, so none is used.  The `s` factor below flips the comparison
// if a case were ever to hand over the two corners in the opposite vertical order.
//
// Notes on the statement:
//  - Only the toy coordinates are unordered; the partitions are guaranteed sorted,
//    which is exactly what the binary search relies on - without it a linear scan or
//    an explicit sort would be needed.
//  - "No toy will land exactly on a cardboard partition", so cross == 0 never occurs
//    on real data and the tie-breaking in the search is arbitrary.  Toys on the box
//    boundary itself are legal and count as inside.
//  - Output format: bins are separated by newlines and consecutive problems by a
//    single blank line - printed before each case except the first, so there is no
//    trailing blank line.
//  - Input ends at a line holding a single 0, which is the n field of a would-be case.

#include <cstdio>
#include <cstdlib>

static const int MAXN = 5005;

static long long U[MAXN], L[MAXN];
static int cnt[MAXN];

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
    int first = 1;
    while (readInt(n)) {
        if (n == 0) break;
        if (!readInt(m)) break;
        readInt(x1); readInt(y1); readInt(x2); readInt(y2);
        long long dy = (long long)y2 - (long long)y1;
        /* the statement puts (x1,y1) at the upper-left, so y1 > y2 and the
           cross product below is positive to the right of a partition; keep
           the test valid even if a case ever hands us the flipped order */
        long long s = (dy < 0) ? 1 : -1;
        int i;
        for (i = 0; i < n; ++i) {
            int u, l;
            readInt(u); readInt(l);
            U[i] = u; L[i] = l;
        }
        for (i = 0; i <= n; ++i) cnt[i] = 0;
        for (int j = 0; j < m; ++j) {
            int px, py;
            readInt(px); readInt(py);
            /* smallest i such that the toy lies strictly left of partition i */
            int lo = 0, hi = n; /* hi == n means left of no partition */
            while (lo < hi) {
                int mid = (lo + hi) / 2;
                long long cross = ((L[mid] - U[mid]) * ((long long)py - (long long)y1)
                                 - dy * ((long long)px - U[mid])) * s;
                if (cross < 0) hi = mid; else lo = mid + 1;
            }
            ++cnt[lo];
        }
        if (!first) putchar('\n');
        first = 0;
        for (i = 0; i <= n; ++i) printf("%d: %d\n", i, cnt[i]);
    }
    return 0;
}
