// POJ 3977 - Subset
// Model: claude-opus-5
//
// Meet in the middle. N <= 35, |a_i| <= 1e15, so any subset sum fits in a
// signed 64-bit integer (35e15 < 9.2e18). Split the array into halves of
// n1 = N/2 and n2 = N - n1 <= 18 elements; every subset of the whole array is
// one subset of each half.
//
//   * Enumerate all 2^n2 subsets of the second half with a lowbit DP, keep only
//     the NON-EMPTY ones, sort by (sum, count) and keep one entry per distinct
//     sum -- the one with the fewest elements.
//   * Enumerate all 2^n1 subsets of the first half (including the empty one).
//     For each, binary search the deduplicated table for -sum; only the
//     predecessor and the successor of that target can minimise |total|, since
//     the stored sums are strictly increasing, so two probes suffice and ties
//     in |total| are resolved by the stored minimal count.
//   * A subset lying entirely in the first half is scored separately, which is
//     exactly the "empty partner" case.
//
// Answer key is (|sum|, |subset|) compared lexicographically.
//
// Two traps, both raised on the discuss board and both reproduced locally:
//   * The second half's table must exclude the empty subset. Deduplicating by
//     sum while keeping the minimum count would otherwise let the empty subset
//     (sum 0, count 0) shadow a genuine zero-sum non-empty subset, and would
//     also allow the illegal empty-empty pairing. On "3 / 1 0 -1" the answer is
//     "0 1" (the element 0 alone); keeping the empty entry prints "0 0".
//   * abs() must be written by hand for 64-bit values; the library overload
//     picked in C++98 can be the int one and silently truncate.
// I/O goes through iostream because POJ's compilers disagree about %lld.
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long LL;

static LL myabs(LL x) { return x < 0 ? -x : x; }

struct Ent {
    LL s;
    int c;
};

static bool entLess(const Ent &x, const Ent &y) {
    if (x.s != y.s) return x.s < y.s;
    return x.c < y.c;
}

static const int HALF = 19;
static const int MAXH = 1 << HALF;

static LL a[40];
static LL sumA[MAXH];
static int cntA[MAXH];
static LL sumB[MAXH];
static int cntB[MAXH];
static Ent ent[MAXH];
static LL bs[MAXH];
static int bc[MAXH];
static int lowidx[40];

int main() {
    for (int k = 0; k < 21; k++) lowidx[(1 << k) % 37] = k;

    int n;
    while (cin >> n && n != 0) {
        for (int i = 0; i < n; i++) cin >> a[i];

        int n1 = n / 2;
        int n2 = n - n1;
        int m1 = 1 << n1;
        int m2 = 1 << n2;

        sumB[0] = 0;
        cntB[0] = 0;
        for (int mask = 1; mask < m2; mask++) {
            int low = mask & (-mask);
            int idx = lowidx[low % 37];
            sumB[mask] = sumB[mask ^ low] + a[n1 + idx];
            cntB[mask] = cntB[mask ^ low] + 1;
        }
        /* Non-empty subsets of the second half only: an empty partner is
           handled separately, and keeping (0,0) here would shadow a genuine
           zero-sum non-empty subset during dedup. */
        int nb = 0;
        for (int mask = 1; mask < m2; mask++) {
            ent[nb].s = sumB[mask];
            ent[nb].c = cntB[mask];
            nb++;
        }
        sort(ent, ent + nb, entLess);
        int nd = 0;
        for (int i = 0; i < nb; i++) {
            if (nd > 0 && bs[nd - 1] == ent[i].s) continue; /* sorted: first is min count */
            bs[nd] = ent[i].s;
            bc[nd] = ent[i].c;
            nd++;
        }

        sumA[0] = 0;
        cntA[0] = 0;
        for (int mask = 1; mask < m1; mask++) {
            int low = mask & (-mask);
            int idx = lowidx[low % 37];
            sumA[mask] = sumA[mask ^ low] + a[idx];
            cntA[mask] = cntA[mask ^ low] + 1;
        }

        LL bestAbs = -1;
        int bestCnt = 0;
        for (int mask = 0; mask < m1; mask++) {
            LL sa = sumA[mask];
            int ca = cntA[mask];
            if (mask != 0) {
                LL v = myabs(sa);
                if (bestAbs < 0 || v < bestAbs || (v == bestAbs && ca < bestCnt)) {
                    bestAbs = v;
                    bestCnt = ca;
                }
            }
            LL target = -sa;
            int pos = (int)(lower_bound(bs, bs + nd, target) - bs);
            for (int d = -1; d <= 0; d++) {
                int j = pos + d;
                if (j < 0 || j >= nd) continue;
                LL v = myabs(sa + bs[j]);
                int c = ca + bc[j];
                if (bestAbs < 0 || v < bestAbs || (v == bestAbs && c < bestCnt)) {
                    bestAbs = v;
                    bestCnt = c;
                }
            }
        }

        cout << bestAbs << " " << bestCnt << "\n";
    }
    return 0;
}
