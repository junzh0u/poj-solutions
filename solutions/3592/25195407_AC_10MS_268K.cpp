// POJ 3592 - Instantaneous Transference
// Model: claude-opus-5
//
// Approach: longest path on the condensation of the movement digraph.
//   Nodes are the n*m cells (n,m <= 40, so at most 1600).  From a passable
//   cell there is an edge east and south (skipped when the neighbour is '#'),
//   and from a '*' cell an extra edge to its declared target.  Teleports are
//   the only way to create a cycle; inside a strongly connected component the
//   truck can reach every cell and come back, so it empties the whole
//   component.  Tarjan (iterative, so the 1600-deep chain cannot blow the
//   stack) condenses the graph and numbers components in reverse topological
//   order, which lets a single increasing pass over component ids do the DP:
//   dp[c] = ore(c) + max(dp[c']) over cross edges c -> c'.  Answer is
//   dp[comp of (0,0)].  Max ore is 40*40*9 = 14400, so int is ample.
//
// Statement points the samples do not pin down (settled from the discuss
// board, then verified against its posted cases):
//   * A '*' cell keeps its ordinary east/south moves - using the magic is
//     optional, not forced.
//   * A teleport target may be a '#'.  Rock can never be occupied, so that
//     edge is simply not added.  (This is harmless either way: '#' carries no
//     ore and has no outgoing edge, so landing on one could never help.  What
//     would matter is letting the truck drive *off* a rock it was teleported
//     onto; the statement forbids standing on rock at all, and the board
//     agrees, so out-edges from '#' are not added.)
//   * A teleport target may fall outside the grid in some posted data, so it
//     is range-checked before the edge is added.
//   * The K target lines follow the '*' cells in row-major ("north to south
//     then west to east") order, and each is given as (row, column).
//   * The destination is anywhere, not the southeast corner.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1600 + 5;
const int MAXE = 3 * 1600 + 10;

int n, m;
char grid[45][64];

int head[MAXN], nxt[MAXE], eto[MAXE], ecnt;
int val[MAXN];

static void addEdge(int u, int v) { eto[ecnt] = v; nxt[ecnt] = head[u]; head[u] = ecnt++; }

int dfn[MAXN], low[MAXN], comp[MAXN], stk[MAXN], top, idx, ncomp;
bool instk[MAXN];
int callStack[MAXN], edgeIter[MAXN];

static void tarjan(int N) {
    for (int i = 0; i < N; i++) { dfn[i] = 0; comp[i] = -1; instk[i] = false; }
    idx = 0; ncomp = 0; top = 0;
    for (int s = 0; s < N; s++) {
        if (dfn[s]) continue;
        int sp = 0;
        dfn[s] = low[s] = ++idx; stk[top++] = s; instk[s] = true; edgeIter[s] = head[s];
        callStack[sp] = s;
        while (sp >= 0) {
            int u = callStack[sp];
            if (edgeIter[u] != -1) {
                int e = edgeIter[u]; edgeIter[u] = nxt[e];
                int v = eto[e];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++idx; stk[top++] = v; instk[v] = true; edgeIter[v] = head[v];
                    callStack[++sp] = v;
                } else if (instk[v]) {
                    if (dfn[v] < low[u]) low[u] = dfn[v];
                }
            } else {
                if (low[u] == dfn[u]) {
                    for (;;) { int w = stk[--top]; instk[w] = false; comp[w] = ncomp; if (w == u) break; }
                    ncomp++;
                }
                sp--;
                if (sp >= 0) { int p = callStack[sp]; if (low[u] < low[p]) low[p] = low[u]; }
            }
        }
    }
}

int compVal[MAXN], dp[MAXN];
int cstart[MAXN + 1], order[MAXN];
int starR[MAXN], starC[MAXN];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        scanf("%d %d", &n, &m);
        for (int i = 0; i < n; i++) scanf("%s", grid[i]);

        int N = n * m;
        for (int i = 0; i < N; i++) head[i] = -1;
        ecnt = 0;

        int K = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                char c = grid[i][j];
                int id = i * m + j;
                if (c == '#') { val[id] = 0; continue; }
                val[id] = (c == '*') ? 0 : (c - '0');
                if (c == '*') { starR[K] = i; starC[K] = j; K++; }
                if (i + 1 < n && grid[i + 1][j] != '#') addEdge(id, (i + 1) * m + j);
                if (j + 1 < m && grid[i][j + 1] != '#') addEdge(id, i * m + j + 1);
            }

        for (int k = 0; k < K; k++) {
            int r, c;
            scanf("%d %d", &r, &c);
            if (r >= 0 && r < n && c >= 0 && c < m && grid[r][c] != '#')
                addEdge(starR[k] * m + starC[k], r * m + c);
        }

        tarjan(N);

        for (int c = 0; c < ncomp; c++) compVal[c] = 0;
        for (int u = 0; u < N; u++) compVal[comp[u]] += val[u];

        /* bucket the nodes by component id (counting sort) */
        for (int c = 0; c <= ncomp; c++) cstart[c] = 0;
        for (int u = 0; u < N; u++) cstart[comp[u] + 1]++;
        for (int c = 0; c < ncomp; c++) cstart[c + 1] += cstart[c];
        {
            int pos[MAXN];
            for (int c = 0; c < ncomp; c++) pos[c] = cstart[c];
            for (int u = 0; u < N; u++) order[pos[comp[u]]++] = u;
        }

        /* Tarjan numbers components in reverse topological order: every cross
           edge c -> c' has c' < c, so a single increasing pass is a valid DP. */
        for (int c = 0; c < ncomp; c++) {
            int best = 0;
            for (int p = cstart[c]; p < cstart[c + 1]; p++) {
                int u = order[p];
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    int cv = comp[eto[e]];
                    if (cv != c && dp[cv] > best) best = dp[cv];
                }
            }
            dp[c] = compVal[c] + best;
        }

        printf("%d\n", dp[comp[0]]);
    }
    return 0;
}
