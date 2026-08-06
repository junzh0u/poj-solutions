// POJ 1679 - The Unique MST
//
// Given a connected undirected graph, decide whether its minimum spanning
// tree is unique; print the MST cost if so, else "Not Unique!".
//
// Approach: Kruskal once to get an MST (cost `best`, edge set `used`), then
// for every edge of that MST rerun Kruskal with that single edge forbidden.
// If some rerun still spans the same number of edges at the same total cost,
// a different edge set achieves the minimum and the MST is not unique. This
// is sound by the exchange property: if two distinct MSTs exist they differ
// in at least one edge, and any MST edge missing from the other one is found
// by the rerun that forbids it.
//
// Cost: sort m edges once, then at most n reruns of an O(m alpha) scan.
// With n <= 100 and hence m <= 4950, twenty cases run in ~0.03s locally,
// far inside the 1000MS / 10000K limits.
//
// Statement notes / edge cases:
//  - Uniqueness is uniqueness of the edge *set*. Two spanning trees with the
//    same shape but different (equal-weight) edges count as two MSTs.
//  - m is not bounded explicitly, so the edge array is allocated per case.
//  - n = 1 with no edges is legal: cost 0, unique.
//  - Self-loops and parallel edges are excluded by the statement but are
//    handled anyway (a self-loop is never taken; duplicate parallel edges of
//    equal weight correctly report "Not Unique!").
//  - Although the input is promised to be connected, the same code computes
//    the minimum spanning forest and its uniqueness for disconnected input,
//    since a rerun that increases the component count is rejected.
//
// Verified against a brute force that enumerates every maximal-forest edge
// subset, over 800 random graphs with n <= 7, tied weights, self-loops,
// parallel edges and disconnected inputs.

#include <cstdio>
#include <cstdlib>
#include <algorithm>

struct Edge {
    int u, v, w;
};

static bool byW(const Edge &a, const Edge &b) { return a.w < b.w; }

static int par[105];

static int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

/* Kruskal skipping edge index `skip` (-1 skips nothing).
   Returns total cost; *cnt receives the number of edges taken,
   and if `used` is non-null the taken edge indices are stored there. */
static long kruskal(const Edge *e, int m, int n, int skip, int *cnt, int *used) {
    for (int i = 1; i <= n; ++i) par[i] = i;
    long cost = 0;
    int c = 0;
    for (int i = 0; i < m; ++i) {
        if (i == skip) continue;
        int a = find(e[i].u), b = find(e[i].v);
        if (a == b) continue;
        par[a] = b;
        cost += e[i].w;
        if (used) used[c] = i;
        ++c;
        if (c == n - 1) break;
    }
    *cnt = c;
    return cost;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t-- > 0) {
        int n, m;
        if (scanf("%d %d", &n, &m) != 2) return 0;
        Edge *e = (Edge *)malloc(sizeof(Edge) * (m > 0 ? m : 1));
        for (int i = 0; i < m; ++i)
            scanf("%d %d %d", &e[i].u, &e[i].v, &e[i].w);
        std::sort(e, e + m, byW);

        int *used = (int *)malloc(sizeof(int) * (n > 0 ? n : 1));
        int cnt = 0;
        long best = kruskal(e, m, n, -1, &cnt, used);

        bool unique = true;
        for (int k = 0; k < cnt; ++k) {
            int cnt2 = 0;
            long cost2 = kruskal(e, m, n, used[k], &cnt2, 0);
            if (cnt2 == cnt && cost2 == best) { unique = false; break; }
        }

        if (unique) printf("%ld\n", best);
        else printf("Not Unique!\n");

        free(used);
        free(e);
    }
    return 0;
}
