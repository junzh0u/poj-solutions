// POJ 3189 - Steady Cow Assignment
// Model: claude-opus-5
//
// Each cow ranks all B barns: the r-th number on her line is the barn she
// ranks r-th.  An assignment's "range" is (max rank used) - (min rank used) + 1,
// and we want it minimal subject to every barn's capacity.
//
// A window of ranks [L,R] is feasible iff there is an assignment where every
// cow lands on a barn she ranks somewhere in [L,R].  That is a bipartite
// b-matching / flow feasibility test: S -> cow (cap 1), cow -> barn pref[i][r]
// for L <= r <= R (cap 1), barn -> T (cap capacity).  Feasible iff maxflow = N.
// The answer is the smallest R-L+1 over feasible windows.
//
// Feasibility is monotone in R for fixed L, so for each L we start at R = L and
// widen, ADDING the new rank's edges to the same residual network and only
// augmenting further -- a flow stays valid when edges are added, so each L costs
// one max flow of value <= N in total rather than one per R.  With the
// R-L+1 >= best pruning this runs in a few milliseconds.
//
// Reading note: on a preference line, position = rank and value = barn number
// (the discuss board's standard misreading is to swap the two).  Input is read
// to EOF; the board's accepted code does the same.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const int MAXB = 25;
const int MAXV = MAXN + MAXB + 2;
const int MAXE = 2 * (MAXN + MAXN * MAXB + MAXB + 10);

int head[MAXV], nxt[MAXE], eto[MAXE], ecap[MAXE], ecnt;
int lev[MAXV], it[MAXV], que[MAXV];
int nv, S, T;

void initGraph(int n) {
    nv = n;
    ecnt = 0;
    for (int i = 0; i < n; i++) head[i] = -1;
}

void addEdge(int u, int v, int c) {
    eto[ecnt] = v; ecap[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
    eto[ecnt] = u; ecap[ecnt] = 0; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

bool bfs() {
    for (int i = 0; i < nv; i++) lev[i] = -1;
    int qh = 0, qt = 0;
    que[qt++] = S; lev[S] = 0;
    while (qh < qt) {
        int u = que[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (ecap[e] > 0 && lev[eto[e]] < 0) {
                lev[eto[e]] = lev[u] + 1;
                que[qt++] = eto[e];
            }
        }
    }
    return lev[T] >= 0;
}

int dfs(int u, int f) {
    if (u == T) return f;
    for (int &e = it[u]; e != -1; e = nxt[e]) {
        int v = eto[e];
        if (ecap[e] > 0 && lev[v] == lev[u] + 1) {
            int d = dfs(v, min(f, ecap[e]));
            if (d > 0) { ecap[e] -= d; ecap[e ^ 1] += d; return d; }
        }
    }
    return 0;
}

int maxflow() {
    int fl = 0;
    while (bfs()) {
        for (int i = 0; i < nv; i++) it[i] = head[i];
        int f;
        while ((f = dfs(S, 1 << 30)) > 0) fl += f;
    }
    return fl;
}

int pref[MAXN][MAXB];
int cp[MAXB];

int main() {
    int n, b;
    while (scanf("%d %d", &n, &b) == 2) {
        for (int i = 0; i < n; i++)
            for (int r = 1; r <= b; r++) scanf("%d", &pref[i][r]);
        for (int j = 1; j <= b; j++) scanf("%d", &cp[j]);

        // [1,b] is always feasible: every cow may use any barn and the
        // capacities sum to at least n.
        int ans = b;
        for (int L = 1; L <= b && ans > 1; L++) {
            S = 0; T = n + b + 1;
            initGraph(n + b + 2);
            for (int i = 0; i < n; i++) addEdge(S, 1 + i, 1);
            for (int j = 1; j <= b; j++) addEdge(n + j, T, cp[j]);
            int flow = 0;
            for (int R = L; R <= b && R - L + 1 < ans; R++) {
                for (int i = 0; i < n; i++) addEdge(1 + i, n + pref[i][R], 1);
                flow += maxflow();
                if (flow == n) { ans = R - L + 1; break; }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
