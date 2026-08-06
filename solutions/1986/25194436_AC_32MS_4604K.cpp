// POJ 1986 - Distance Queries
//
// The farms form a tree (the input is the one from "Navigation Nightmare":
// N farms, M roads, each road "F1 F2 L D" with a compass letter D).  For a
// query (u, v) the path in a tree is unique, so
//
//     answer = dist(root,u) + dist(root,v) - 2 * dist(root,lca(u,v))
//
// where dist(root,x) is the weighted root distance.  With N, M <= 40000 and
// K <= 10000 queries a per-query traversal is too slow, so LCA is answered by
// binary lifting: one pass computes parent/depth/root-distance, then
// par[k][v] = par[k-1][par[k-1][v]] for k < 17 (2^16 > 40000).
//
// Notes on the statement / input:
//  * The direction letter carries no information for distance, but it must
//    still be consumed by the reader.
//  * The tree can be arbitrarily deep (a 40000-node chain), so the traversal
//    is an explicit BFS queue, never recursion, which would blow the stack.
//    Filling the lifting table in BFS order guarantees a node's parent is
//    already processed.
//  * The graph is treated as a forest (BFS restarted from every unvisited
//    node) so a disconnected input cannot leave stale state; the outer loop
//    also accepts several test cases concatenated in one file.
//  * Distances fit in a 32-bit int: 40000 edges * length 1000 = 4*10^7.
#include <cstdio>
#include <cstring>

static const int MAXN = 40005;
static const int MAXM = 40005;
static const int LOG = 17;

static int head[MAXN], nxt[2 * MAXM], to[2 * MAXM], wt[2 * MAXM];
static int ecnt;

static int par[LOG][MAXN];
static int depth_[MAXN];
static int dist_[MAXN];
static int order_[MAXN];
static bool vis[MAXN];

static char buf[1 << 16];
static int bl, bp;

static int gc() {
    if (bp == bl) {
        bl = (int)fread(buf, 1, sizeof(buf), stdin);
        bp = 0;
        if (bl <= 0) return -1;
    }
    return buf[bp++];
}

/* reads a non-negative integer; returns 0 on EOF */
static int readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return 0;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return 1;
}

/* skips whitespace then reads one non-whitespace character (the direction letter) */
static int readLetter() {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    return c;
}

static void addEdge(int a, int b, int w) {
    to[ecnt] = b; wt[ecnt] = w; nxt[ecnt] = head[a]; head[a] = ecnt++;
}

int main() {
    int n, m;
    while (readInt(n)) {
        if (!readInt(m)) break;
        ecnt = 0;
        for (int i = 1; i <= n; i++) head[i] = -1;
        for (int i = 0; i < m; i++) {
            int a, b, w;
            readInt(a); readInt(b); readInt(w);
            readLetter(); /* direction: irrelevant to distance, but must be consumed */
            addEdge(a, b, w);
            addEdge(b, a, w);
        }

        for (int i = 1; i <= n; i++) { vis[i] = false; depth_[i] = 0; dist_[i] = 0; }
        for (int k = 0; k < LOG; k++) par[k][0] = 0;

        /* iterative BFS over every component (input is a forest in general) */
        int cnt = 0;
        for (int s = 1; s <= n; s++) {
            if (vis[s]) continue;
            vis[s] = true;
            par[0][s] = 0;
            depth_[s] = 0;
            dist_[s] = 0;
            int qh = cnt;
            order_[cnt++] = s;
            while (qh < cnt) {
                int u = order_[qh++];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int v = to[e];
                    if (vis[v]) continue;
                    vis[v] = true;
                    par[0][v] = u;
                    depth_[v] = depth_[u] + 1;
                    dist_[v] = dist_[u] + wt[e];
                    order_[cnt++] = v;
                }
            }
        }

        /* binary lifting tables, filled in BFS order so parents come first */
        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i < cnt; i++) {
                int u = order_[i];
                par[k][u] = par[k - 1][par[k - 1][u]];
            }
        }

        int q;
        if (!readInt(q)) q = 0;
        for (int i = 0; i < q; i++) {
            int u, v;
            readInt(u); readInt(v);
            int a = u, b = v;
            if (depth_[a] < depth_[b]) { int t = a; a = b; b = t; }
            int diff = depth_[a] - depth_[b];
            for (int k = 0; k < LOG; k++)
                if (diff & (1 << k)) a = par[k][a];
            if (a != b) {
                for (int k = LOG - 1; k >= 0; k--)
                    if (par[k][a] != par[k][b]) { a = par[k][a]; b = par[k][b]; }
                a = par[0][a];
            }
            printf("%d\n", dist_[u] + dist_[v] - 2 * dist_[a]);
        }
    }
    return 0;
}
