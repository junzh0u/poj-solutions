// POJ 3925 - Minimal Ratio Tree
// Model: claude-sonnet-5
// Approach: n<=15, m<=n. Enumerate all m-subsets of the n nodes in
// lexicographic (ascending) order via recursive combination generation.
// For a fixed subset the node-weight sum is fixed, so minimizing the
// ratio sum(edge)/sum(node) over spanning trees of that subset reduces
// to minimizing sum(edge) alone, i.e. computing a minimum spanning tree
// (Prim's, O(m^2)) on the induced subgraph. Compare candidate ratios
// exactly via cross multiplication (a/b < c/d  <=>  a*d < c*b) to avoid
// floating point issues; the board flags this exact pitfall plus an
// overflow trap from seeding the "best" sentinel too large - here we
// simply track whether a best answer exists yet instead of using a
// giant sentinel value, so no cross product ever overflows (edge sums
// and node sums are each bounded by 15*100=1500, so any product used
// in comparisons fits easily in a 64-bit, even 32-bit, integer).
// Subsets are generated in lexicographic order of the sorted node
// index tuple, which is exactly the tie-break order the statement
// asks for (smallest node number first, then next, ...), so keeping
// the first subset achieving the current best ratio (strict '<' only)
// automatically yields the required tie-break.
#include <cstdio>
#include <cstring>

int n, m;
int nodeW[16];
int edgeW[16][16];

int chosen[16];       // chosen[i] = node index (0-based) currently selected
int bestChosen[16];
bool haveBest;
int bestNum, bestDen; // best ratio = bestNum/bestDen (edge sum / node sum)

// Prim's MST on the induced subgraph formed by chosen[0..m-1]; returns
// sum of edge weights of MST (subset size m, connected complete graph
// induced, so MST always exists for m>=1).
int mstEdgeSum() {
    static bool inTree[16];
    static int dist[16];
    for (int i = 0; i < m; i++) { inTree[i] = false; dist[i] = 1 << 29; }
    dist[0] = 0;
    int total = 0;
    for (int iter = 0; iter < m; iter++) {
        int u = -1;
        for (int i = 0; i < m; i++) {
            if (!inTree[i] && (u == -1 || dist[i] < dist[u])) u = i;
        }
        inTree[u] = true;
        total += dist[u];
        int nu = chosen[u];
        for (int v = 0; v < m; v++) {
            if (!inTree[v]) {
                int nv = chosen[v];
                int w = edgeW[nu][nv];
                if (w < dist[v]) dist[v] = w;
            }
        }
    }
    return total;
}

void evalSubset() {
    int nodeSum = 0;
    for (int i = 0; i < m; i++) nodeSum += nodeW[chosen[i]];
    int edgeSum = mstEdgeSum();
    if (!haveBest || edgeSum * bestDen < bestNum * nodeSum) {
        haveBest = true;
        bestNum = edgeSum;
        bestDen = nodeSum;
        for (int i = 0; i < m; i++) bestChosen[i] = chosen[i];
    }
}

void combine(int start, int depth) {
    if (depth == m) {
        evalSubset();
        return;
    }
    // remaining slots to fill: m - depth; need indices from start..n-1
    int remain = m - depth;
    for (int i = start; i <= n - remain; i++) {
        chosen[depth] = i;
        combine(i + 1, depth + 1);
    }
}

int main() {
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 0; i < n; i++) scanf("%d", &nodeW[i]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &edgeW[i][j]);
        haveBest = false;
        combine(0, 0);
        for (int i = 0; i < m; i++) {
            printf("%d%s", bestChosen[i] + 1, (i + 1 < m) ? " " : "\n");
        }
    }
    return 0;
}
