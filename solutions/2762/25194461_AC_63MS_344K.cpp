// POJ 2762 - Going from u to v or from v to u?
//
// A cave is good iff every pair of rooms is *comparable* under reachability:
// for all u, v either u reaches v or v reaches u. That is weaker than strong
// connectivity (the sample chain 1->2->3 with no way back would still qualify)
// and strictly stronger than the graph being connected.
//
// Contract each strongly connected component: inside a component every pair is
// mutually reachable, so the condition reduces to the condensation DAG, where
// comparability of every pair means the DAG has a Hamiltonian path -- all
// components lie on one chain.
//
// A DAG has a Hamiltonian path iff its topological order is unique, and in that
// case any topological order *is* that path. Tarjan numbers components in
// reverse topological order, so ncomp, ncomp-1, ..., 1 is already a valid
// topological order; it suffices to check that some original edge joins
// component k to component k-1 for every k. O(n + m) per test case.
//
// Notes on the statement: the corridors are one-way even though the wording
// says "connecting room u and room v"; the sample and the problem title settle
// the direction as u -> v. Self-loops and duplicate edges are harmless here.
// n < 1001 but the DFS can be 1000 deep, so Tarjan is written iteratively.
#include <cstdio>

const int MAXN = 1005;
const int MAXM = 6005;

int head[MAXN], nxt[MAXM], dst[MAXM], ecnt;
int dfn[MAXN], low[MAXN], comp[MAXN], stk[MAXN], instk[MAXN];
int callStack[MAXN], edgeIter[MAXN];
int linked[MAXN];
int n, m, idx, top, ncomp;

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return -1;
        c = getchar();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sgn;
}

static void tarjan() {
    idx = 0; top = 0; ncomp = 0;
    int i;
    for (i = 1; i <= n; ++i) { dfn[i] = 0; comp[i] = 0; instk[i] = 0; }
    for (int s = 1; s <= n; ++s) {
        if (dfn[s]) continue;
        int sp = 0;
        dfn[s] = low[s] = ++idx;
        stk[top++] = s; instk[s] = 1;
        callStack[0] = s; edgeIter[0] = head[s];
        while (sp >= 0) {
            int u = callStack[sp];
            if (edgeIter[sp] != -1) {
                int e = edgeIter[sp];
                edgeIter[sp] = nxt[e];
                int v = dst[e];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++idx;
                    stk[top++] = v; instk[v] = 1;
                    ++sp;
                    callStack[sp] = v; edgeIter[sp] = head[v];
                } else if (instk[v]) {
                    if (dfn[v] < low[u]) low[u] = dfn[v];
                }
            } else {
                if (low[u] == dfn[u]) {
                    ++ncomp;
                    for (;;) {
                        int w = stk[--top];
                        instk[w] = 0;
                        comp[w] = ncomp;
                        if (w == u) break;
                    }
                }
                --sp;
                if (sp >= 0) {
                    int p = callStack[sp];
                    if (low[u] < low[p]) low[p] = low[u];
                }
            }
        }
    }
}

int main() {
    int T = readInt();
    while (T-- > 0) {
        n = readInt();
        m = readInt();
        int i;
        for (i = 1; i <= n; ++i) head[i] = -1;
        ecnt = 0;
        for (i = 0; i < m; ++i) {
            int u = readInt(), v = readInt();
            dst[ecnt] = v; nxt[ecnt] = head[u]; head[u] = ecnt; ++ecnt;
        }
        tarjan();
        for (i = 1; i <= ncomp; ++i) linked[i] = 0;
        /* Tarjan numbers components in reverse topological order, so
           ncomp, ncomp-1, ..., 1 is a topological order of the condensation.
           A Hamiltonian path exists iff every consecutive pair in it is joined. */
        for (int u = 1; u <= n; ++u) {
            for (int e = head[u]; e != -1; e = nxt[e]) {
                int v = dst[e];
                if (comp[u] == comp[v] + 1) linked[comp[v]] = 1;
            }
        }
        int ok = 1;
        for (i = 1; i < ncomp; ++i) if (!linked[i]) { ok = 0; break; }
        puts(ok ? "Yes" : "No");
    }
    return 0;
}
