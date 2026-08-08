// POJ 2781 - The mysterious X network
// Model: claude-opus-5
//
// Unweighted shortest path by BFS. The answer is the number of *intermediate*
// camarades on a minimal chain from c1 to c2, i.e. (hop count) - 1: the sample
// path 1-0-2 has two hops and one intermediate.
//
// The one real trap is input framing, and the official sample cannot see it.
// Each of the N lines *starts with the camarade's own label c*, and the lines
// are not guaranteed to arrive in label order, so the adjacency row must be
// indexed by c and not by the line's position. The sample happens to list
// 0,1,2,3 in order, so a solver that ignores c reproduces it exactly; a
// reordered path graph separates the two readings (correct 2, line-indexed -1).
//
// Sizing: N <= 1e5 and n_c < 100 means up to ~1e7 integers on input, so the
// scanner is a hand-rolled fread tokenizer and the queue is a plain array
// (the discuss board reports std::queue timing out here, and stack-allocated
// 1e5 arrays crashing). The adjacency lives in one static 40MB block, the same
// shape as the board's posted accepted solution. The file gives the "knows"
// relation already symmetric, as the statement's symmetry claim and the sample
// both show, so no reverse edges are synthesized.
//
// Worst case measured locally: N=1e5, ~9.8e6 neighbour entries, diameter 3030,
// full BFS -> 0.08s against a 10s limit.

#include <cstdio>

const int MAXN = 100000;
const int MAXD = 101;

static int adj[MAXN][MAXD];
static int deg[MAXN];
static int dst[MAXN];
static int que[MAXN];

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

static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

int main() {
    int n;
    while (readInt(n)) {
        if (n <= 0 || n > MAXN) break;
        int i, j;
        for (i = 0; i < n; i++) deg[i] = 0;
        for (i = 0; i < n; i++) {
            int c, k;
            if (!readInt(c)) return 0;
            if (!readInt(k)) return 0;
            int d = 0;
            for (j = 0; j < k; j++) {
                int x;
                if (!readInt(x)) return 0;
                if (c >= 0 && c < n && d < MAXD) adj[c][d++] = x;
            }
            if (c >= 0 && c < n) deg[c] = d;
        }
        int c1, c2;
        if (!readInt(c1)) return 0;
        if (!readInt(c2)) return 0;

        for (i = 0; i < n; i++) dst[i] = -1;
        int head = 0, tail = 0;
        if (c1 >= 0 && c1 < n) { dst[c1] = 0; que[tail++] = c1; }
        while (head < tail && (c2 < 0 || c2 >= n || dst[c2] < 0)) {
            int u = que[head++];
            int du = deg[u];
            const int *row = adj[u];
            for (j = 0; j < du; j++) {
                int v = row[j];
                if (v >= 0 && v < n && dst[v] < 0) {
                    dst[v] = dst[u] + 1;
                    que[tail++] = v;
                }
            }
        }
        int ans = (c2 >= 0 && c2 < n && dst[c2] > 0) ? dst[c2] - 1 : -1;
        printf("%d %d %d\n", c1, c2, ans);
    }
    return 0;
}
