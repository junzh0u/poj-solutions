// POJ 1167 - The Buses
// Model: claude-opus-5
//
// A route runs "at regular intervals from 12:00 to 12:59 throughout the entire
// hour", so it is fixed by (s, d) with stops s, s+d, ... <= 59 and 0 <= s < d:
// a bus at s-d >= 0 would have been noted too, so the first noted stop is the
// first stop.  "Each bus route stops at least 2 times" gives s+d <= 59, hence
// s <= 29.  The man noted every bus, so the multiset union of the chosen routes
// must equal the noted multiset *exactly* -- a route is usable only while all
// of its stops still have unconsumed arrivals.  Minimise the number of routes.
// (The Description's "output the starting time and the interval" is dead text;
// the Output section and the sample ask only for the count.)
//
// Two facts drive the pruning.  (a) Availability is monotone: consuming stops
// never makes a route usable again, so the routes usable later are a subset of
// those usable now.  (b) Every route covering the smallest still-uncovered
// minute t starts exactly at t -- its own first stop is <= t and must still be
// available -- so exactly ct[t] of the remaining routes start there.  Together
// they give a packing bound: at most ct[u] remaining routes start at u, none
// longer than the longest still-available route starting at u, so the k longest
// of those capacities must sum to at least the number of stops left.
//
// Search A canonicalises a solution as a non-increasing sequence of route
// lengths, which makes "sum + ceil(n/len) >= best" cut the scan, and keys a
// transposition table on (residual counts, previous length) -- state-determined
// because the length cap is carried explicitly.  The table is what makes the
// weak canonicalisation affordable: the many orders of equal-length routes all
// collapse onto one entry.  When nothing longer than a pair remains, every
// remaining route has length exactly 2, so their order is free and fact (b) can
// be imposed on top, forcing the next route to start at t (that alone took the
// worst case found from 1.5s to 0.03s).  Search A settles every instance from
// the discuss board in under 2500 nodes.
//
// Its one bad regime is a mix of length-2 and length-3 routes, where search B
// -- routes in non-decreasing start order, always covering the smallest
// uncovered minute next -- is far better, so B runs as a bounded fallback.
// They share one table under disjoint key spaces to stay inside the 10MB limit.
//
// The statement's "at most 17 routes" is used only as the initial incumbent; if
// it is ever violated the whole search is redone uncapped, so this is a correct
// general solver either way.

#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Route { int s, d, len; unsigned mlo, mhi; };
static Route rt[1000];
static int nrt;
static int ct[60];
static int n, ans;
static long nodes, budget;
static bool aborted;

static unsigned zlo, zhi;                 /* bit i set <=> minute i is used up */
static unsigned Mlo[30][60], Mhi[30][60]; /* stop mask of route (s,d) */
static int Len[30][60];

static unsigned Z[60][301][2], ZL[62][2], ZB[2];
static unsigned h0, h1;
#define TBITS 19
#define TSIZE (1 << TBITS)
struct Ent { unsigned k0, k1; short bound; };
static Ent tab[TSIZE];

static int cmpRoute(const void *a, const void *b) {
    return ((const Route *)b)->len - ((const Route *)a)->len;
}

#define AVAIL(s, d) (((Mlo[s][d] & zlo) | (Mhi[s][d] & zhi)) == 0u)
#define RAVAIL(k)   (((rt[k].mlo & zlo) | (rt[k].mhi & zhi)) == 0u)

#define TAKE(s, d, used)                                                    \
    for (j = (s); j < 60; j += (d)) {                                       \
        h0 ^= Z[j][ct[j]][0] ^ Z[j][ct[j] - 1][0];                          \
        h1 ^= Z[j][ct[j]][1] ^ Z[j][ct[j] - 1][1];                          \
        if (--ct[j] == 0) { if (j < 30) zlo |= 1u << j; else zhi |= 1u << (j - 30); } \
        (used)++;                                                           \
    }

#define GIVEBACK(s, d)                                                      \
    for (j = (s); j < 60; j += (d)) {                                       \
        if (ct[j]++ == 0) { if (j < 30) zlo &= ~(1u << j); else zhi &= ~(1u << (j - 30)); } \
        h0 ^= Z[j][ct[j]][0] ^ Z[j][ct[j] - 1][0];                          \
        h1 ^= Z[j][ct[j]][1] ^ Z[j][ct[j] - 1][1];                          \
    }

/* fewest routes of length <= cap that could still cover the n remaining stops;
   1000 when the state is already unsatisfiable */
static int lowerBound(int t, int cap) {
    int byLen[62];
    memset(byLen, 0, sizeof(byLen));
    for (int u = t; u <= 29; u++) {
        if (ct[u] == 0) continue;
        int best = 0;
        for (int d = u + 1; d <= 59 - u; d++)
            if (Len[u][d] <= cap && AVAIL(u, d)) { best = Len[u][d]; break; }
        if (u == t && best == 0) return 1000;
        if (best) byLen[best] += ct[u];
    }
    int lb = 0, need = n;
    for (int len = cap; len >= 2 && need > 0; len--) {
        if (!byLen[len]) continue;
        int want = (need + len - 1) / len;
        int take = byLen[len] < want ? byLen[len] : want;
        lb += take;
        need -= take * len;
    }
    return need > 0 ? 1000 : lb;
}

/* ---- search A: routes in non-increasing length order ---- */
static void dfsA(int lprev, int sum) {
    if (n == 0) { if (sum < ans) ans = sum; return; }
    int rem = ans - sum;
    if (rem <= 0) return;
    if (++nodes > budget) { aborted = true; return; }

    unsigned g0 = h0 ^ ZL[lprev][0], g1 = h1 ^ ZL[lprev][1];
    unsigned slot = g0 & (TSIZE - 1);
    if (tab[slot].k0 == g0 && tab[slot].k1 == g1 && tab[slot].bound >= rem) return;

    int t = 0;
    while (ct[t] == 0) t++;
    int v;
    if (t > 29) {
        v = 1000;
    } else {
        int lb = lowerBound(t, lprev);
        if (lb >= rem) {
            v = lb;
        } else {
            int lmax = 0, k;
            for (k = 0; k < nrt; k++) {
                if (rt[k].len > lprev || rt[k].len > n) continue;
                if (RAVAIL(k)) { lmax = rt[k].len; break; }
            }
            for (k = 0; k < nrt; k++) {
                if (rt[k].len > lprev || rt[k].len > n) continue;
                if (lmax == 2 && rt[k].s != t) continue;   /* order is free here */
                if (sum + (n + rt[k].len - 1) / rt[k].len >= ans) break;
                if (!RAVAIL(k)) continue;
                int j, used = 0;
                TAKE(rt[k].s, rt[k].d, used)
                n -= used;
                dfsA(rt[k].len, sum + 1);
                GIVEBACK(rt[k].s, rt[k].d)
                n += used;
                if (aborted) return;      /* partial: nothing proven, store nothing */
            }
            v = ans - sum;
        }
    }
    if (tab[slot].k0 != g0 || tab[slot].k1 != g1 || tab[slot].bound < v) {
        tab[slot].k0 = g0; tab[slot].k1 = g1; tab[slot].bound = (short)v;
    }
}

/* ---- search B: routes in non-decreasing start order ---- */
static void dfsB(int sum) {
    if (n == 0) { if (sum < ans) ans = sum; return; }
    int rem = ans - sum;
    if (rem <= 0) return;
    if (++nodes > budget) { aborted = true; return; }

    unsigned g0 = h0 ^ ZB[0], g1 = h1 ^ ZB[1];
    unsigned slot = g0 & (TSIZE - 1);
    if (tab[slot].k0 == g0 && tab[slot].k1 == g1 && tab[slot].bound >= rem) return;

    int t = 0;
    while (ct[t] == 0) t++;
    int v;
    if (t > 29) {
        v = 1000;
    } else {
        int lb = lowerBound(t, 60);
        if (lb >= rem) {
            v = lb;
        } else {
            for (int d = t + 1; d <= 59 - t; d++) {
                if (!AVAIL(t, d)) continue;
                int j, used = 0;
                TAKE(t, d, used)
                n -= used;
                dfsB(sum + 1);
                GIVEBACK(t, d)
                n += used;
                if (aborted) return;
            }
            v = ans - sum;
        }
    }
    if (tab[slot].k0 != g0 || tab[slot].k1 != g1 || tab[slot].bound < v) {
        tab[slot].k0 = g0; tab[slot].k1 = g1; tab[slot].bound = (short)v;
    }
}

static int solve(int cap) {
    ans = cap;
    memset(tab, 0, sizeof(tab));

    static const long sched[4] = {5000000L, 20000000L, 100000000L, 500000000L};
    for (int i = 0; i < 4; i++) {
        budget = sched[i];
        nodes = 0; aborted = false;
        if (i & 1) dfsB(0); else dfsA(60, 0);   /* A first: it needs < 2500 nodes
                                                   on every discuss-board case */
        if (!aborted) return ans;
    }
    budget = 2000000000L;
    nodes = 0; aborted = false; dfsA(60, 0);
    return ans;
}

int main() {
    unsigned seed = 123456789u;
    for (int t = 0; t < 60; t++)
        for (int c = 0; c <= 300; c++)
            for (int q = 0; q < 2; q++) {
                seed = seed * 1103515245u + 12345u;
                Z[t][c][q] = seed ^ (seed >> 13) ^ (seed << 7);
            }
    for (int l = 0; l < 62; l++)
        for (int q = 0; q < 2; q++) {
            seed = seed * 1103515245u + 12345u;
            ZL[l][q] = seed ^ (seed >> 13) ^ (seed << 7);
        }
    for (int q = 0; q < 2; q++) {
        seed = seed * 1103515245u + 12345u;
        ZB[q] = seed ^ (seed >> 13) ^ (seed << 7);
    }
    for (int s = 0; s <= 29; s++)
        for (int d = s + 1; d <= 59 - s; d++) {
            unsigned lo = 0, hi = 0;
            int c = 0;
            for (int i = s; i < 60; i += d) {
                if (i < 30) lo |= 1u << i; else hi |= 1u << (i - 30);
                c++;
            }
            Mlo[s][d] = lo; Mhi[s][d] = hi; Len[s][d] = c;
        }

    int m;
    while (scanf("%d", &m) == 1) {
        memset(ct, 0, sizeof(ct));
        for (int i = 0; i < m; i++) {
            int v;
            if (scanf("%d", &v) != 1) return 0;
            ct[v]++;
        }
        n = m;
        zlo = zhi = 0;
        for (int i = 0; i < 60; i++)
            if (ct[i] == 0) { if (i < 30) zlo |= 1u << i; else zhi |= 1u << (i - 30); }
        h0 = h1 = 0;
        for (int i = 0; i < 60; i++) { h0 ^= Z[i][ct[i]][0]; h1 ^= Z[i][ct[i]][1]; }

        nrt = 0;
        for (int s = 0; s <= 29; s++) {
            if (ct[s] == 0) continue;
            for (int d = s + 1; d <= 59 - s; d++) {
                if (!AVAIL(s, d)) continue;
                rt[nrt].s = s; rt[nrt].d = d; rt[nrt].len = Len[s][d];
                rt[nrt].mlo = Mlo[s][d]; rt[nrt].mhi = Mhi[s][d];
                nrt++;
            }
        }
        qsort(rt, nrt, sizeof(rt[0]), cmpRoute);

        int r = solve(18);
        if (r == 18) r = solve(m + 1);   /* statement's <=17 guarantee violated */
        printf("%d\n", r);
    }
    return 0;
}
