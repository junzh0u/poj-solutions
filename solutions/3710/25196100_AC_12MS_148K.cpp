// POJ 3710 - Christmas Game
// Model: claude-fable-5
//
// Green Hackenbush on a forest of trees with pendant simple polygons
// (each polygon touches the main tree at exactly one node, each edge is
// in at most one polygon). By the Fusion Principle a cycle of L edges
// fuses to L loops = Grundy (L mod 2): an odd polygon acts as a single
// edge, an even polygon vanishes. Colon Principle then gives
// g(v) = XOR over non-cycle children (g(child)+1). Cycle tree edges are
// fused without the +1 (robust even if something hangs off a cycle
// vertex). The board confirms: edges are undirected, parallel edges do
// occur (a 2-cycle, worth 0), and the roots are independent - the
// answer is the XOR of every root's Grundy value; Sally (first player)
// wins iff it is nonzero.
#include <cstdio>
#include <cstring>

const int MAXN = 105;
const int MAXE = 1100; // 2 * 500 directed entries

int head[MAXN], nxt[MAXE], to[MAXE], ecnt;
int dep[MAXN], par[MAXN], parEdge[MAXN], g[MAXN];
bool used[MAXE / 2 + 4], cyc[MAXE / 2 + 4], vis[MAXN];

void addEdge(int a, int b) { to[ecnt] = b; nxt[ecnt] = head[a]; head[a] = ecnt++; }

void dfs(int u)
{
    vis[u] = true;
    g[u] = 0;
    for (int e = head[u]; e != -1; e = nxt[e]) {
        int id = e >> 1;
        if (used[id]) continue;
        used[id] = true;
        int w = to[e];
        if (!vis[w]) {
            dep[w] = dep[u] + 1; par[w] = u; parEdge[w] = id;
            dfs(w);
            if (cyc[id]) g[u] ^= g[w];      // fused cycle vertex, no +1
            else         g[u] ^= g[w] + 1;  // ordinary branch
        } else {
            // back edge to an ancestor (or self loop): cycle of len edges
            int len = dep[u] - dep[w] + 1;
            if (len & 1) g[w] ^= 1;         // L loops -> parity
            for (int x = u; x != w; x = par[x]) cyc[parEdge[x]] = true;
        }
    }
}

int main()
{
    int n;
    while (scanf("%d", &n) == 1) {
        int total = 0;
        for (int t = 0; t < n; t++) {
            int m, k;
            scanf("%d %d", &m, &k);
            ecnt = 0;
            memset(head, -1, sizeof(head));
            memset(used, 0, sizeof(used));
            memset(cyc, 0, sizeof(cyc));
            memset(vis, 0, sizeof(vis));
            for (int i = 0; i < k; i++) {
                int a, b;
                scanf("%d %d", &a, &b);
                addEdge(a, b);
                addEdge(b, a);
            }
            (void)m;
            dep[1] = 0; par[1] = 1; parEdge[1] = -1;
            dfs(1);
            total ^= g[1];
        }
        printf(total ? "Sally\n" : "Harry\n");
    }
    return 0;
}
