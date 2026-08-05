// POJ 1990 - MooFest
//
// Every pair (i, j) of the N cows converses at volume max(v(i), v(j)) * |x(i) - x(j)|;
// sum that over all N(N-1)/2 pairs.
//
// Sort the cows by volume ascending. Then when they are processed in that order, the
// current cow is the louder one of every pair it forms with an already-processed cow,
// so its own v is the max for all of those pairs and the pair is charged exactly once.
// Equal volumes are the only ambiguous case -- "the larger of the two" is then either
// cow -- but the tie-break in the sort fixes an order, and since max(v, v) = v the
// value charged is the same whichever of the two is treated as louder, so each tied
// pair is still counted once with the right volume.
//
// What the current cow needs is the total distance to all earlier cows:
//   sum |x - x(j)| = (cntLeft * x - sumLeft) + (sumRight - cntRight * x)
// Two Fenwick trees indexed by coordinate (1..20000), one holding counts and one
// holding the sum of coordinates, give the left count and left coordinate-sum in
// O(log C); the right side is the running totals minus the left. One pass, O(n log C).
//
// The answer overflows 32 bits -- the maximum is around 2.7e16 (all v = 20000, cows
// packed on 1..20000) -- so it is accumulated in long long and printed digit by digit
// rather than with %lld, whose spelling differs between POJ's compilers.
//
// Statement notes: coordinates are stated to be distinct, but the Fenwick prefix query
// at index x would include a cow sitting exactly at x contributing distance 0 anyway,
// so duplicate coordinates would also be handled correctly.
#include <cstdio>
#include <cstdlib>
#include <algorithm>

static const int MAXC = 20000;

struct Cow {
    int v, x;
};

static bool byV(const Cow& a, const Cow& b) {
    if (a.v != b.v) return a.v < b.v;
    return a.x < b.x;
}

static int cntBit[MAXC + 1];
static long long sumBit[MAXC + 1];

static void addBit(int i, int c, long long s) {
    for (; i <= MAXC; i += i & (-i)) { cntBit[i] += c; sumBit[i] += s; }
}

static void qryBit(int i, int& c, long long& s) {
    c = 0; s = 0;
    for (; i > 0; i -= i & (-i)) { c += cntBit[i]; s += sumBit[i]; }
}

static void printLL(long long v) {
    char buf[32];
    int n = 0;
    if (v == 0) { putchar('0'); putchar('\n'); return; }
    if (v < 0) { putchar('-'); v = -v; }
    while (v > 0) { buf[n++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (n > 0) putchar(buf[--n]);
    putchar('\n');
}

static Cow cows[20005];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) {
        if (scanf("%d %d", &cows[i].v, &cows[i].x) != 2) return 0;
    }
    std::sort(cows, cows + n, byV);

    long long ans = 0;
    long long totalSum = 0;
    int totalCnt = 0;

    for (int i = 0; i < n; ++i) {
        int x = cows[i].x;
        int lc; long long ls;
        qryBit(x, lc, ls);           /* cows with coordinate <= x (x unique, so < x) */
        int rc = totalCnt - lc;
        long long rs = totalSum - ls;
        long long dist = ((long long)lc * x - ls) + (rs - (long long)rc * x);
        ans += (long long)cows[i].v * dist;
        addBit(x, 1, (long long)x);
        totalCnt += 1;
        totalSum += x;
    }

    printLL(ans);
    return 0;
}
