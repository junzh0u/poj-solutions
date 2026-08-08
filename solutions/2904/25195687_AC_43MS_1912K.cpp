// POJ 2904 - The Mailboxes Manufacturers Problem
// Model: claude-opus-5
//
// Egg-drop where the cost is the number of crackers burnt, not the number of
// tests.  The unknown is t = the largest load the prototype survives, so
// t is in [0, m] (t = 0 means one cracker already destroys it, t = m means it
// survives a full load).  Loading x crackers costs x whatever happens; the box
// survives iff t >= x, otherwise it is destroyed and one mailbox is spent.
//
// f[k][lo][hi] = worst-case crackers still needed when t is known to lie in
// [lo, hi] and k intact mailboxes remain:
//   f[k][lo][lo] = 0,   f[0][lo][hi] = INF for hi > lo,
//   f[k][lo][hi] = min over x in [lo+1, hi] of
//                    x + max(f[k][x][hi], f[k-1][lo][x-1]).
// Survival leaves [x, hi] (t >= x), not [x+1, hi] -- the boundary the discuss
// board's posted recurrence gets wrong.  Only x in [lo+1, hi] is worth testing:
// x <= lo is a guaranteed survival and x > hi a guaranteed explosion, so either
// burns crackers for no information.
//
// Answer f[min(k,10)][0][m]; extra mailboxes beyond ceil(log2(m+1)) never help,
// so clamping k is safe.  Table sized to 200 rather than the stated m <= 100
// as cheap headroom (the board reports Runtime Errors from tight arrays);
// 10 * 200 * 200 / 2 * 200 inner steps still run in well under the limit.
//
// Verified against a from-the-statement reference that keeps the candidate set
// explicit (no contiguity assumed) and tries every splitting x: identical on
// all 400 pairs k = 1..10, m = 1..40, and on k = 1,2,3,5,7,10 at m = 99/100.

#include <stdio.h>

#define MAXM 200
#define INF 1000000000

static int f[11][MAXM + 2][MAXM + 2];

int main() {
    int k, lo, hi, len, x, a, b, w, best, n, m;

    for (k = 0; k <= 10; ++k)
        for (lo = 0; lo <= MAXM; ++lo)
            f[k][lo][lo] = 0;
    for (lo = 0; lo <= MAXM; ++lo)
        for (hi = lo + 1; hi <= MAXM; ++hi)
            f[0][lo][hi] = INF;

    for (k = 1; k <= 10; ++k) {
        for (len = 1; len <= MAXM; ++len) {
            for (lo = 0; lo + len <= MAXM; ++lo) {
                hi = lo + len;
                best = INF;
                for (x = lo + 1; x <= hi; ++x) {
                    a = f[k][x][hi];
                    b = f[k - 1][lo][x - 1];
                    w = a > b ? a : b;
                    if (w >= INF) continue;
                    if (x + w < best) best = x + w;
                }
                f[k][lo][hi] = best;
            }
        }
    }

    if (scanf("%d", &n) != 1) return 0;
    while (n-- > 0) {
        if (scanf("%d %d", &k, &m) != 2) break;
        if (k > 10) k = 10;
        if (k < 0) k = 0;
        if (m > MAXM) m = MAXM;
        if (m < 0) m = 0;
        printf("%d\n", f[k][0][m]);
    }
    return 0;
}
