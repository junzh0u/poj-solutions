// POJ 3377 - Ferry Lanes
// Model: claude-opus-5
//
// The docks form a ladder: 2*(N+1) nodes, north walk na[i] and south walk
// sb[i] between columns i and i+1, ferry fc[i] across column i.  Shortest
// path from one node to another, N up to 1e6 and several test cases, so an
// explicit graph plus Dijkstra is wasteful; O(N) per case is available
// because the ladder pins the shape of an optimal path.
//
// Structure argument.  The cut between columns i and i+1 holds exactly two
// edges, na[i] and sb[i], so a simple path crosses it at most twice.  Let
// L and R be the two endpoint columns.  Cuts strictly inside [L,R] separate
// start from finish, so they are crossed an odd number of times, hence
// exactly once: the path is monotone across the whole span.  Cuts outside
// are crossed an even number of times, so 0 or 2, and the doubly-crossed
// ones form one contiguous run hanging off column L (to the west) or column
// R (to the east).  Such a detour walks both banks of its span in full and
// uses exactly one ferry, at its far end; a second ferry would close a
// cycle.  So a detour is nothing but an alternative way to change banks at
// the endpoint column:
//
//   xl = min over k <= L of ( sum na[k..L-1] + sum sb[k..L-1] + fc[k] )
//   xr = min over k >= R of ( sum na[R..k-1] + sum sb[R..k-1] + fc[k] )
//
// (k = L and k = R give the plain ferries, so xl <= fc[L], xr <= fc[R].)
// Seed the DP at column L with 0 on the start's bank and xl on the other,
// sweep east to column R relaxing the ferry at each column, and finish with
// min(arrive on the finish's bank, arrive on the other bank + xr).  Start
// and finish are swapped first if the start lies east, since the path is
// undirected.
//
// Skipping the endpoint detours is the trap: the discuss board posts the
// case N=4 / (0,1)->(1,3) / na = 0 100 0 0 / fc = 0 100 0 100 0 /
// sb = 0 0 100 0, where both detours are free and the answer is 0 while a
// detour-free DP says 200.  The official sample does not distinguish them.
//
// Verified by exhaustive enumeration against a plain Dijkstra on the
// explicit ladder: every weight assignment and every endpoint pair for
// N<=2 over {0,1,2}, N=3 over {0,1,3}, N=4 over {0,1} -- 4.7M cases, all
// agreeing -- plus randomized trials up to N=200.  Six mutants (each detour
// dropped, no swap of banks when the start lies east, banks read in the
// wrong order, no mid-span ferry, final answer ignoring xr) were all killed
// by the enumeration; the official sample kills only one of them.
//
// G++ rather than C++ because the answer needs 64 bits, and POJ's C++
// compiler has neither long long nor %lld; the board also reports the two
// compilers differing tenfold in speed here.  Input dominates the runtime
// (3N+1 numbers per case), so it is read through a buffered scanner that
// skips arbitrary whitespace -- the board warns of runs of spaces between
// numbers, and the posted case carries a blank line.

#include <cstdio>

static const int MAXN = 1000006;
static int na[MAXN], sb[MAXN], fc[MAXN];

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static inline bool readInt(long long &out) {
    int ch = gc();
    while (ch != -1 && (ch < '0' || ch > '9') && ch != '-') ch = gc();
    if (ch == -1) return false;
    int sg = 1;
    if (ch == '-') { sg = -1; ch = gc(); }
    long long x = 0;
    while (ch >= '0' && ch <= '9') { x = x * 10 + (ch - '0'); ch = gc(); }
    out = sg * x;
    return true;
}

int main() {
    long long v;
    while (readInt(v) && v != 0) {
        int N = (int)v;
        long long ds, ps, dt, pt;
        readInt(ds); readInt(ps); readInt(dt); readInt(pt);
        int i;
        for (i = 0; i < N; ++i) { readInt(v); na[i] = (int)v; }
        for (i = 0; i <= N; ++i) { readInt(v); fc[i] = (int)v; }
        for (i = 0; i < N; ++i) { readInt(v); sb[i] = (int)v; }

        int rs = (int)ds, L = (int)ps, rt = (int)dt, R = (int)pt;
        if (L > R) { int t = L; L = R; R = t; t = rs; rs = rt; rt = t; }

        /* Cheapest bank change at column L, detour to the west allowed. */
        long long accA = 0, accB = 0, xl = fc[L];
        for (i = L - 1; i >= 0; --i) {
            accA += na[i];
            accB += sb[i];
            long long cand = accA + accB + fc[i];
            if (cand < xl) xl = cand;
        }
        /* Cheapest bank change at column R, detour to the east allowed. */
        accA = 0; accB = 0;
        long long xr = fc[R];
        for (i = R + 1; i <= N; ++i) {
            accA += na[i - 1];
            accB += sb[i - 1];
            long long cand = accA + accB + fc[i];
            if (cand < xr) xr = cand;
        }

        long long d0, d1;
        if (rs == 0) { d0 = 0; d1 = xl; } else { d1 = 0; d0 = xl; }
        for (i = L + 1; i <= R; ++i) {
            long long e0 = d0 + na[i - 1];
            long long e1 = d1 + sb[i - 1];
            long long ci = fc[i];
            d0 = (e0 < e1 + ci) ? e0 : e1 + ci;
            d1 = (e1 < e0 + ci) ? e1 : e0 + ci;
        }

        long long same = (rt == 0) ? d0 : d1;
        long long other = (rt == 0) ? d1 : d0;
        long long ans = same < other + xr ? same : other + xr;
        printf("%lld\n", ans);
    }
    return 0;
}
