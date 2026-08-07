// POJ 1719 - Shooting Contest
// Model: claude-opus-5
//
// Each column holds exactly two white squares, and we must hit exactly one white
// square per column so that every one of the r rows is hit at least once.  Read
// the columns as edges of a multigraph on the r rows: column i is the edge
// {a_i, b_i}, and choosing which white square to hit orients that edge toward
// one endpoint.  The requirement "no row without a white square being hit" is
// then "every vertex has in-degree >= 1" in the orientation.
//
// Such an orientation exists iff every connected component has at least as many
// edges as vertices, i.e. no component is a tree (and no row is isolated).  Per
// component: BFS a spanning tree; if no extra (non-tree) edge exists the
// component is a tree -> NO.  Otherwise take one extra edge e0 = {u, v}, assign
// e0 to u, and re-BFS the component from u while skipping e0 (still connected,
// since the spanning tree survives), assigning each tree edge to the child it
// discovers.  Every vertex but u is then covered by its own tree edge and u by
// e0; any remaining edge is assigned to either endpoint.
//
// Ambiguity settled from the statement and the discuss board: a row may be hit
// more than once (only "at least one hit per row" is required, not a matching
// that uses each row once), and the negative answer is the exact word "NO".
// Both BFS passes are iterative, so there is no recursion depth risk; all counts
// fit in int (r, c <= 1000).

#include <cstdio>

const int MAXR = 2005;
const int MAXC = 2005;

int ea[MAXC], eb[MAXC];
int head[MAXR], nxt[2 * MAXC], dst[2 * MAXC];
int ans[MAXC];
bool vis[MAXR], tree_edge[MAXC], assigned[MAXC];
int queue_[MAXR], comp[MAXR];

int r, c;

int main() {
    int x;
    if (scanf("%d", &x) != 1) return 0;
    while (x-- > 0) {
        if (scanf("%d %d", &r, &c) != 2) break;
        int i;
        for (i = 0; i < MAXR; ++i) { head[i] = -1; vis[i] = false; }
        int ec = 0;
        for (i = 1; i <= c; ++i) {
            int u, v;
            scanf("%d %d", &u, &v);
            ea[i] = u; eb[i] = v;
            tree_edge[i] = false; assigned[i] = false; ans[i] = u;
            dst[ec] = v; nxt[ec] = head[u]; head[u] = ec; ++ec;
            dst[ec] = u; nxt[ec] = head[v]; head[v] = ec; ++ec;
        }
        bool ok = true;
        for (i = 1; i <= r && ok; ++i) {
            if (vis[i]) continue;
            if (head[i] == -1) { ok = false; break; }  /* row never white */
            /* first BFS: spanning tree of the component */
            int qh = 0, qt = 0, nc = 0;
            queue_[qt++] = i; vis[i] = true; comp[nc++] = i;
            while (qh < qt) {
                int u = queue_[qh++];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int w = dst[e], id = (e >> 1) + 1;
                    if (!vis[w]) {
                        vis[w] = true; tree_edge[id] = true;
                        queue_[qt++] = w; comp[nc++] = w;
                    }
                }
            }
            /* a non-tree edge inside the component <=> edges >= vertices */
            int extra = -1;
            for (int k = 0; k < nc && extra < 0; ++k) {
                int u = comp[k];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int id = (e >> 1) + 1;
                    if (!tree_edge[id]) { extra = id; break; }
                }
            }
            if (extra < 0) { ok = false; break; }  /* component is a tree */
            /* second BFS rooted at an endpoint of the extra edge, skipping it */
            int root = ea[extra];
            ans[extra] = root; assigned[extra] = true;
            for (int k = 0; k < nc; ++k) vis[comp[k]] = false;
            qh = qt = 0;
            queue_[qt++] = root; vis[root] = true;
            while (qh < qt) {
                int u = queue_[qh++];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int w = dst[e], id = (e >> 1) + 1;
                    if (id == extra) continue;
                    if (!vis[w]) {
                        vis[w] = true;
                        ans[id] = w; assigned[id] = true;
                        queue_[qt++] = w;
                    }
                }
            }
            for (int k = 0; k < nc; ++k) vis[comp[k]] = true;
        }
        if (!ok) {
            printf("NO\n");
        } else {
            for (i = 1; i <= c; ++i) printf(i == 1 ? "%d" : " %d", ans[i]);
            printf("\n");
        }
    }
    return 0;
}
