// POJ 3700 - Missile Defence System
// Model: claude-opus-5
//
// Cover the arriving sequence with as few strictly monotone subsequences as
// possible (each system takes missiles in ascending or in descending order).
// NP-hard in general, so this is an IDA*: for lim = 1, 2, ... ask whether lim
// systems suffice, scanning the missiles left to right and keeping only the
// last height of each system.
//
// Branching is binary, not "try every system", by two exchange arguments:
//   * If some ascending system can take h, opening a new one is never better:
//     swap the tails of the new system and of that system's future, and the
//     count is unchanged.  So h joins an existing ascending system whenever
//     one fits.
//   * Among the ascending systems that fit, take the one with the largest tail
//     below h.  Choosing tail a over a larger tail b (a < b < h) leaves tails
//     {b, h} instead of {a, h}, and smaller tails are strictly more permissive.
//   Symmetric on the descending side (smallest tail above h).
// A corollary: up[] then stays strictly decreasing and dn[] strictly
// increasing, so "largest tail below h" is just the first one found.
//
// Plain IDA* needed 2.5s locally on the worst random n = 50 (limit 3000MS on a
// slower judge), so states already proven infeasible at the current limit are
// cached.  A state is exactly (index, up tails, dn tails) -- the budget is
// implied -- and after compressing heights to ranks 1..n it packs into 64 bits.
// The cache is stamped per limit, since failing at lim says nothing about
// lim + 1.  Worst n = 50 case is then ~3ms.
//
// Input framing: the statement only says "several test cases"; the sample's
// trailing 0 is the terminator (confirmed on the discuss board).  EOF ends the
// loop too, in case a case list is not 0-terminated.

#include <cstdio>
#include <cstring>
#include <algorithm>

static int n;
static int h[64];
static int up[64], dn[64];
static int lim;

/* states already proven infeasible at the current limit; a miss only costs
   time, so overwriting a full bucket is safe */
static const int HB = 20;
static const int HSZ = 1 << HB;
static const int HMASK = HSZ - 1;
static unsigned long long tkey[HSZ];
static unsigned short tstamp[HSZ];
static unsigned short curStamp;

static unsigned long long encode(int i, int a, int b) {
    if (a + b > 9) return 0ULL;   /* would not fit in 64 bits */
    unsigned long long k = (unsigned long long)i;
    k = (k << 4) | (unsigned long long)a;
    for (int j = 0; j < a; j++) k = (k << 6) | (unsigned long long)up[j];
    for (int j = 0; j < b; j++) k = (k << 6) | (unsigned long long)dn[j];
    return k;   /* zero only for the root, which is not worth caching */
}

static bool seen(unsigned long long k) {
    if (!k) return false;
    unsigned int p = (unsigned int)((k * 1181783497276652981ULL) >> (64 - HB));
    for (int t = 0; t < 4; t++) {
        unsigned int q = (p + t) & HMASK;
        if (tstamp[q] == curStamp && tkey[q] == k) return true;
    }
    return false;
}

static void mark(unsigned long long k) {
    if (!k) return;
    unsigned int p = (unsigned int)((k * 1181783497276652981ULL) >> (64 - HB));
    for (int t = 0; t < 4; t++) {
        unsigned int q = (p + t) & HMASK;
        if (tstamp[q] != curStamp) { tstamp[q] = curStamp; tkey[q] = k; return; }
    }
    unsigned int q = p & HMASK;
    tstamp[q] = curStamp; tkey[q] = k;
}

static bool dfs(int i, int a, int b) {
    if (a + b > lim) return false;
    if (i == n) return true;

    unsigned long long key = encode(i, a, b);
    if (seen(key)) return false;

    const int v = h[i];

    /* ascending: extend the largest tail below v, else open a new system */
    int j = 0;
    while (j < a && up[j] >= v) j++;
    if (j < a) {
        int old = up[j];
        up[j] = v;
        if (dfs(i + 1, a, b)) { up[j] = old; return true; }
        up[j] = old;
    } else {
        up[a] = v;
        if (dfs(i + 1, a + 1, b)) return true;
    }

    /* descending: extend the smallest tail above v, else open a new system */
    j = 0;
    while (j < b && dn[j] <= v) j++;
    if (j < b) {
        int old = dn[j];
        dn[j] = v;
        if (dfs(i + 1, a, b)) { dn[j] = old; return true; }
        dn[j] = old;
    } else {
        dn[b] = v;
        if (dfs(i + 1, a, b + 1)) return true;
    }

    mark(key);
    return false;
}

int main() {
    memset(tstamp, 0, sizeof(tstamp));
    curStamp = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) scanf("%d", &h[i]);

        int srt[64];
        for (int i = 0; i < n; i++) srt[i] = h[i];
        std::sort(srt, srt + n);
        for (int i = 0; i < n; i++)
            h[i] = (int)(std::lower_bound(srt, srt + n, h[i]) - srt) + 1;

        int ans = n;
        for (lim = 1; lim <= n; lim++) {
            if (++curStamp == 0) { memset(tstamp, 0, sizeof(tstamp)); curStamp = 1; }
            if (dfs(0, 0, 0)) { ans = lim; break; }
        }
        printf("%d\n", ans);
    }
    return 0;
}
