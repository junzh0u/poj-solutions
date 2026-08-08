// POJ 2152 - Fire
// Model: claude-opus-5
//
// Minimum-weight distance-constrained dominating set on a tree: every city K
// must have a firehouse within distance D(K) (a city holding one satisfies
// itself, since dist(K,K)=0 <= D(K)), and building in K costs W(K).
//
// Tree DP over "who serves whom".  Root at 1 and let
//   dp[i][j] = min cost of subtree(i) given a firehouse is built at j and j
//              serves i (requires dist(i,j) <= D(i)), W(j) included;
//   best[i]  = min over j of dp[i][j].
// Then dp[i][j] = W[j] + sum over children c of min(best[c], dp[c][j] - W[j]):
// either c is served by the same station j, whose W(j) is already paid here and
// so is subtracted, or c's subtree is solved independently.  Answer best[1].
// Distances come from one BFS per node, so no N x N distance matrix is stored;
// the whole thing is O(N^2) per test case, which the discuss board confirms is
// the intended complexity (the board's other note is that the limit is tight,
// hence the flat arrays, sequential j-loops and buffered reader).
//
// Ambiguity: the statement constrains only cities *without* a firehouse.  The
// stricter reading -- every city needs a *different* city's firehouse within
// D -- is settled against by the official sample: it answers 3 2 4 3 5 there
// instead of 2 1 2 2 3.
//
// Verified against an exhaustive-subset brute force: two whole parameter
// families enumerated (chain of 5 over W in {1,2}^5 x D in {0,1,2}^5, and a
// branching 5-node tree over W in {1,3}^5 x D in {0,1,2,3}^5, 40544 cases) plus
// ~950 random trees; all five mutants of the recurrence die on the sample.
#include <cstdio>

static const int MAXN = 1005;
static const int INF = 1000000000;

static int W[MAXN], D[MAXN];
static int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], len[2 * MAXN];
static int par[MAXN], order[MAXN], parlen[MAXN];
static int dr[MAXN], que[MAXN];
static int dp[MAXN][MAXN];
static int best[MAXN];

static char buf[1 << 16];
static int bufPos = 0, bufLen = 0;

static int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

/* reads the next non-negative integer, skipping anything that is not a digit;
   returns 0 on EOF before any digit, 1 otherwise. */
static int readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return 0;
    int v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10 + (c - '0');
        c = gc();
    }
    out = v;
    return 1;
}

int main() {
    int T;
    if (!readInt(T)) return 0;
    for (int tc = 0; tc < T; ++tc) {
        int n;
        if (!readInt(n)) break;
        for (int i = 1; i <= n; ++i) readInt(W[i]);
        for (int i = 1; i <= n; ++i) readInt(D[i]);
        for (int i = 1; i <= n; ++i) head[i] = -1;
        int ec = 0;
        for (int e = 0; e < n - 1; ++e) {
            int u, v, l;
            readInt(u); readInt(v); readInt(l);
            to[ec] = v; len[ec] = l; nxt[ec] = head[u]; head[u] = ec; ++ec;
            to[ec] = u; len[ec] = l; nxt[ec] = head[v]; head[v] = ec; ++ec;
        }

        /* BFS from node 1 to get a parent array and a processing order */
        int qh = 0, qt = 0;
        order[qt++] = 1; par[1] = 0; parlen[1] = 0;
        while (qh < qt) {
            int u = order[qh++];
            for (int e = head[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                if (v != par[u]) { par[v] = u; parlen[v] = len[e]; order[qt++] = v; }
            }
        }

        /* process nodes bottom-up */
        for (int idx = n - 1; idx >= 0; --idx) {
            int u = order[idx];

            /* distances from u to every node */
            for (int i = 1; i <= n; ++i) dr[i] = -1;
            int h = 0, t = 0;
            que[t++] = u; dr[u] = 0;
            while (h < t) {
                int x = que[h++];
                int dx = dr[x];
                for (int e = head[x]; e != -1; e = nxt[e]) {
                    int y = to[e];
                    if (dr[y] < 0) { dr[y] = dx + len[e]; que[t++] = y; }
                }
            }

            int *du = dp[u];
            int du_ = D[u];
            for (int j = 1; j <= n; ++j) du[j] = (dr[j] <= du_) ? W[j] : INF;

            for (int e = head[u]; e != -1; e = nxt[e]) {
                int c = to[e];
                if (c == par[u]) continue;
                int *dc = dp[c];
                int bc = best[c];
                for (int j = 1; j <= n; ++j) {
                    int alt = dc[j] - W[j];
                    du[j] += (alt < bc) ? alt : bc;
                }
            }

            int b = INF;
            for (int j = 1; j <= n; ++j) if (du[j] < b) b = du[j];
            best[u] = b;
        }

        printf("%d\n", best[1]);
    }
    return 0;
}
