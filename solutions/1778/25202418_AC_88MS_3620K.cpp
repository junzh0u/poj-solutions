// POJ 1778 - All Discs Considered
// Model: claude-sonnet-5
//
// Approach: Each package sits on disc 0 or disc 1. In an optimal installation
// order the sequence of discs used forms maximal monochromatic "blocks" that
// strictly alternate colors (otherwise two adjacent same-color blocks would
// merge). Label blocks by consecutive integers so that a block's color is a
// fixed function of the parity of its index (two choices overall: which
// color starts on an even index -- i.e. which disc is inserted first).
//
// For a fixed choice of "which disc's color occupies even indices", each
// package v must receive an integer level(v) with level(v) % 2 == requiredParity(v)
// (determined by its disc and the choice), and for every dependency edge
// y -> x (y required before x) we need level(y) <= level(x). To minimize the
// number of distinct levels used (= number of blocks), greedily assign each
// node the smallest feasible level given its predecessors, processed in
// topological order (a classic longest-path-under-modular-constraint DP):
//   level(x) = max over predecessors y of adjust(level(y), requiredParity(x))
// where adjust(L, p) = L if L%2==p else L+1, and the base level (no
// predecessors) is simply requiredParity(x) itself (0 or 1).
// Total blocks = max(level) - min(level) + 1.
//
// Doing this is optimal for a fixed parity convention because all
// constraints are pure lower bounds, so minimizing every level pointwise via
// forward topological DP simultaneously minimizes the overall span. There are
// exactly two global conventions (which disc is "inserted first" / occupies
// even-parity blocks); the discuss board independently confirms this exact
// two-way ambiguity ("两个heap，两种可能：先插入dvd 1/2" -- two possibilities:
// insert DVD 1 or DVD 2 first). We compute both and take the smaller block
// count, then answer = blocks + 1 (for the initial insertion and the final
// removal, in addition to the swaps counted as blocks-1).
//
// No statement ambiguity beyond the above, which the sample and the board
// both settle. Verified against the official sample and randomized
// brute-force differential tests on small graphs (exhaustive/topological
// permutation search for true minimum blocks), plus a dedicated stress test
// near N=100000, D=100000 for performance (10s limit, judge machines slower).

#include <cstdio>

const int MAXN = 100005;
const int MAXD = 100005;

int headArr[MAXN];
int nxt[MAXD];
int to_[MAXD];
int indeg[MAXN];
int indegWork[MAXN];
unsigned char color_[MAXN];
int level_[MAXN];
int order_[MAXN];
int queue_[MAXN];
int fromArr[MAXD], toEdgeArr[MAXD];

int main() {
    int N1, N2, D;
    while (scanf("%d %d %d", &N1, &N2, &D) == 3) {
        if (N1 == 0 && N2 == 0 && D == 0) break;
        int N = N1 + N2;

        for (int v = 1; v <= N; v++) {
            headArr[v] = -1;
            indeg[v] = 0;
            color_[v] = (v <= N1) ? 0 : 1;
        }

        for (int i = 0; i < D; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            // package a requires package b installed first: edge b -> a
            fromArr[i] = b;
            toEdgeArr[i] = a;
            to_[i] = a;
            nxt[i] = headArr[b];
            headArr[b] = i;
            indeg[a]++;
        }

        // topological order via Kahn's algorithm
        for (int v = 1; v <= N; v++) indegWork[v] = indeg[v];
        int qh = 0, qt = 0;
        for (int v = 1; v <= N; v++) {
            if (indegWork[v] == 0) queue_[qt++] = v;
        }
        int oc = 0;
        while (qh < qt) {
            int u = queue_[qh++];
            order_[oc++] = u;
            for (int e = headArr[u]; e != -1; e = nxt[e]) {
                int w = to_[e];
                if (--indegWork[w] == 0) queue_[qt++] = w;
            }
        }
        // oc should equal N (no circular dependencies, per statement)

        int bestBlocks = -1;
        for (int convention = 0; convention < 2; convention++) {
            for (int v = 1; v <= N; v++) {
                int reqParity = color_[v] ^ convention;
                level_[v] = reqParity; // smallest nonneg int with that parity
            }
            for (int idx = 0; idx < oc; idx++) {
                int u = order_[idx];
                int lu = level_[u];
                for (int e = headArr[u]; e != -1; e = nxt[e]) {
                    int w = to_[e];
                    int reqParityW = color_[w] ^ convention;
                    int cand = ((lu & 1) == reqParityW) ? lu : lu + 1;
                    if (cand > level_[w]) level_[w] = cand;
                }
            }
            int mn = level_[1], mx = level_[1];
            for (int v = 2; v <= N; v++) {
                if (level_[v] < mn) mn = level_[v];
                if (level_[v] > mx) mx = level_[v];
            }
            int blocks = mx - mn + 1;
            if (bestBlocks == -1 || blocks < bestBlocks) bestBlocks = blocks;
        }

        printf("%d\n", bestBlocks + 1);
    }
    return 0;
}
