// POJ 2553 - The Bottom of a Graph
//
// bottom(G) = { v | every w reachable from v also reaches v }. Reachability
// is transitive, so v qualifies exactly when its whole strongly connected
// component is a sink of the condensation: everything v reaches then lies in
// that same component, and every vertex of a component reaches every other.
// So condense the digraph into SCCs, mark the components with no edge leaving
// them, and print every vertex of every such component in ascending order.
// Unlike 2186 there may be several sink components and all of them count.
//
// SCCs come from Tarjan's algorithm written iteratively with an explicit call
// stack: v is up to 5000 and a chain input would recurse 5000 deep, which
// risks a stack overflow on the judge even though it survives locally.
//
// The out-degree pass walks each vertex's adjacency list and stops at the
// first edge crossing to another component, so only head/next/dst are kept -
// e has no stated upper bound, and dropping the parallel edge arrays halves
// the per-edge memory. Self-loops and duplicate edges are harmless: both are
// intra-component edges that the check skips.
//
// Ambiguities: the statement allows an empty bottom and asks for an empty
// line then, but a finite non-empty digraph's condensation is a DAG and
// always has at least one sink, so that case cannot arise - the code prints
// the (never empty) line uniformly. Input is a stream of test cases ended by
// a single 0, and e may be 0.

#include <cstdio>
#include <vector>

using namespace std;

static inline int readInt(bool &ok) {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) { ok = false; return 0; }
        c = getchar();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    ok = true;
    return x * sgn;
}

static int n, m;
static vector<int> head_, nxt_, dst_;
static vector<int> dfn, low, comp, instk, stk, callv, calle, outdeg;
static int idx_, ncomp, top_;

static void tarjan(int root) {
    int sp = 0;
    dfn[root] = low[root] = ++idx_;
    stk[top_++] = root;
    instk[root] = 1;
    callv[0] = root;
    calle[0] = head_[root];
    while (sp >= 0) {
        int u = callv[sp];
        int e = calle[sp];
        if (e != -1) {
            calle[sp] = nxt_[e];
            int v = dst_[e];
            if (!dfn[v]) {
                dfn[v] = low[v] = ++idx_;
                stk[top_++] = v;
                instk[v] = 1;
                ++sp;
                callv[sp] = v;
                calle[sp] = head_[v];
            } else if (instk[v]) {
                if (dfn[v] < low[u]) low[u] = dfn[v];
            }
        } else {
            if (low[u] == dfn[u]) {
                ++ncomp;
                for (;;) {
                    int w = stk[--top_];
                    instk[w] = 0;
                    comp[w] = ncomp;
                    if (w == u) break;
                }
            }
            --sp;
            if (sp >= 0) {
                int p = callv[sp];
                if (low[u] < low[p]) low[p] = low[u];
            }
        }
    }
}

int main() {
    bool ok;
    for (;;) {
        n = readInt(ok);
        if (!ok || n == 0) break;
        m = readInt(ok);
        if (!ok) break;

        head_.assign(n + 1, -1);
        nxt_.assign(m, -1);
        dst_.assign(m, 0);
        for (int i = 0; i < m; ++i) {
            int a = readInt(ok);
            int b = readInt(ok);
            dst_[i] = b;
            nxt_[i] = head_[a];
            head_[a] = i;
        }

        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        comp.assign(n + 1, 0);
        instk.assign(n + 1, 0);
        stk.assign(n + 1, 0);
        callv.assign(n + 1, 0);
        calle.assign(n + 1, 0);
        idx_ = 0;
        ncomp = 0;
        top_ = 0;
        for (int i = 1; i <= n; ++i)
            if (!dfn[i]) tarjan(i);

        outdeg.assign(ncomp + 1, 0);
        for (int u = 1; u <= n; ++u) {
            int cu = comp[u];
            for (int e = head_[u]; e != -1; e = nxt_[e])
                if (comp[dst_[e]] != cu) { ++outdeg[cu]; break; }
        }

        bool first = true;
        for (int i = 1; i <= n; ++i) {
            if (outdeg[comp[i]] == 0) {
                if (!first) putchar(' ');
                printf("%d", i);
                first = false;
            }
        }
        putchar('\n');
    }
    return 0;
}
