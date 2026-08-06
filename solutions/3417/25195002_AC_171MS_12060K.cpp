// POJ 3417 - Network
// Model: claude-sonnet-5
// Approach: Root the original tree at node 1. Each of the M extra edges (u,v) is
// treated as a path in the tree; using the classic edge-difference trick
// (diff[u]++, diff[v]++, diff[lca(u,v)] -= 2, then a subtree-sum in post order)
// we get, for every tree edge (parent[x],x), how many extra edges' tree-path
// crosses it. Cutting one tree edge e and one extra edge f disconnects the
// network iff:
//   cover(e) == 0  -> f may be ANY of the M extra edges (M ways): e alone
//                     already splits the tree and no extra edge reconnects it
//   cover(e) == 1  -> f must be that one covering edge (1 way)
//   cover(e) >= 2  -> 0 ways (removing only one extra edge still leaves a
//                     second one bridging the two halves)
// Summing this over the N-1 tree edges gives the answer. LCA is answered with
// binary lifting, O((N+M) log N) overall.
// Ambiguity/pitfalls: the discuss board flags that some of the M extra edges
// can be self-loops (u == v) with N possibly 1 (no tree edges at all, answer
// 0); a self-loop's own diff contributions cancel to 0 and are otherwise
// harmless. Adjacency uses a hand-rolled array-based linked list (not
// std::vector) since the board reports vector adjacency getting flagged as
// too slow on this judge for N,M up to 1e5.
#include <cstdio>
#include <cstring>

const int MAXN = 100005;
const int LOG = 18;

int head_[MAXN], nxt_[2 * MAXN], to_[2 * MAXN], ecnt = 0;
static void addEdge(int a, int b) {
    to_[ecnt] = b; nxt_[ecnt] = head_[a]; head_[a] = ecnt++;
    to_[ecnt] = a; nxt_[ecnt] = head_[b]; head_[b] = ecnt++;
}

int up_[MAXN][LOG];
int depth_[MAXN];
int par0[MAXN];
int order_[MAXN];
long long diff_[MAXN];
long long cnt_[MAXN];
bool visited_[MAXN];

int N, M;

int main() {
    scanf("%d %d", &N, &M);
    memset(head_, -1, sizeof(int) * (N + 1));
    for (int i = 0; i < N - 1; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        addEdge(a, b);
    }

    // BFS from node 1 to get parent/depth/visit order
    memset(visited_, 0, sizeof(bool) * (N + 1));
    int qh = 0, qt = 0;
    order_[qt++] = 1;
    visited_[1] = true;
    par0[1] = 0;
    depth_[1] = 0;
    while (qh < qt) {
        int u = order_[qh++];
        for (int e = head_[u]; e != -1; e = nxt_[e]) {
            int v = to_[e];
            if (!visited_[v]) {
                visited_[v] = true;
                par0[v] = u;
                depth_[v] = depth_[u] + 1;
                order_[qt++] = v;
            }
        }
    }

    for (int v = 1; v <= N; v++) up_[v][0] = par0[v];
    for (int k = 1; k < LOG; k++) {
        for (int v = 1; v <= N; v++) {
            int mid = up_[v][k - 1];
            up_[v][k] = (mid == 0) ? 0 : up_[mid][k - 1];
        }
    }

    memset(diff_, 0, sizeof(long long) * (N + 1));

    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        int uu = u, vv = v;
        if (depth_[uu] < depth_[vv]) { int t = uu; uu = vv; vv = t; }
        int d = depth_[uu] - depth_[vv];
        for (int k = 0; k < LOG; k++) {
            if ((d >> k) & 1) uu = up_[uu][k];
        }
        if (uu != vv) {
            for (int k = LOG - 1; k >= 0; k--) {
                if (up_[uu][k] != up_[vv][k]) {
                    uu = up_[uu][k];
                    vv = up_[vv][k];
                }
            }
            uu = up_[uu][0];
        }
        int lca = uu;
        diff_[u] += 1;
        diff_[v] += 1;
        diff_[lca] -= 2;
    }

    for (int v = 1; v <= N; v++) cnt_[v] = diff_[v];
    for (int i = qt - 1; i >= 0; i--) {
        int v = order_[i];
        if (v != 1) cnt_[par0[v]] += cnt_[v];
    }

    long long ans = 0;
    for (int v = 1; v <= N; v++) {
        if (v == 1) continue;
        long long c = cnt_[v];
        if (c == 0) ans += M;
        else if (c == 1) ans += 1;
    }
    printf("%lld\n", ans);
    return 0;
}
