// POJ 3683 - Priest John's Busiest Day
//
// Each wedding's ceremony is one boolean: held at the start [Si, Si+Di] or
// at the end [Ti-Di, Ti]. John can attend at most one ceremony at a time, so
// this is 2-SAT: variable i true means "start-based", false means "end-based".
// For every pair of weddings i, j and every combination of their two choices,
// if the resulting intervals properly overlap (touching at a shared endpoint
// is explicitly allowed by the statement, so the test is strict: is < je and
// js < ie), that combination is forbidden, contributing the clause
// (x_i != a OR x_j != b) as two implications.
//
// Satisfiability is decided the usual way: no variable may share a strongly
// connected component with its own negation, found via an iterative Tarjan
// (n up to 1000 means 2000 literals; recursion could go that deep and blow
// the stack). Unlike a plain yes/no 2-SAT instance, this problem also wants
// a printed schedule, which requires reading off values from the component
// order rather than just checking satisfiability: Tarjan numbers components
// in reverse topological order (the order they finish, sinks first), so
// "positive literal's component ordered after its negative's" in true
// topological order corresponds to the positive literal finishing *first*,
// i.e. x_i = true iff comp[T(i)] < comp[F(i)] under this finish-order
// numbering. Verified against a forced-unit-clause example by hand before
// trusting it, since the direction is easy to flip silently.
//
// n <= 1000 gives ~500K pairs x 4 combinations x 2 edges = up to ~4M
// implication edges; stored as a plain edge list (not a bit matrix) since
// 2000 literals x 4M/2000 avg out-degree is still far cheaper as a list,
// and it fits comfortably under the 64MB limit (peak measured ~33MB on an
// all-identical-interval worst case, 2000 nodes / ~4M edges).
//
// Statement note: times are hh:mm and must be echoed back in the same
// zero-padded format; the only ambiguity is that shared-endpoint ceremonies
// are explicitly not conflicts, confirmed by the sample (08:00-08:30 and
// 08:40-09:00 don't touch, but the statement's "either at the beginning or
// the ending" phrasing plus this repo's other 2-SAT interval problems made
// the touching-is-fine convention worth stating outright rather than assuming).
#include <cstdio>

const int MAXN = 1005;
const int MAXV = 2 * MAXN;
const int MAXE = 4000010;

int head[MAXV], nxt[MAXE], to_[MAXE], ecnt;

static void addEdge(int u, int v) {
    to_[ecnt] = v; nxt[ecnt] = head[u]; head[u] = ecnt++;
}

int dfn[MAXV], low[MAXV], comp[MAXV], stk[MAXV], stkTop;
bool onStk[MAXV];
int idx_, ncomp;
int callU[MAXV], callE[MAXV];

static void tarjan(int root) {
    int sp = 0;
    callU[0] = root; callE[0] = head[root];
    dfn[root] = low[root] = ++idx_;
    stk[stkTop++] = root; onStk[root] = true;
    while (sp >= 0) {
        int u = callU[sp];
        int e = callE[sp];
        if (e != -1) {
            callE[sp] = nxt[e];
            int v = to_[e];
            if (dfn[v] == 0) {
                dfn[v] = low[v] = ++idx_;
                stk[stkTop++] = v; onStk[v] = true;
                ++sp;
                callU[sp] = v; callE[sp] = head[v];
            } else if (onStk[v]) {
                if (dfn[v] < low[u]) low[u] = dfn[v];
            }
        } else {
            if (low[u] == dfn[u]) {
                ++ncomp;
                for (;;) {
                    int w = stk[--stkTop];
                    onStk[w] = false;
                    comp[w] = ncomp;
                    if (w == u) break;
                }
            }
            --sp;
            if (sp >= 0) {
                int p = callU[sp];
                if (low[u] < low[p]) low[p] = low[u];
            }
        }
    }
}

int As[MAXN], Ae[MAXN], Bs[MAXN], Be[MAXN]; /* choice-0 (start) and choice-1 (end) intervals, in minutes */
int Sh[MAXN], Sm[MAXN], Th[MAXN], Tm[MAXN], Dd[MAXN];

static inline int T(int i) { return 2 * i; }     /* literal: variable i takes choice 0 (start-based) */
static inline int F(int i) { return 2 * i + 1; } /* literal: variable i takes choice 1 (end-based) */

static inline void intervalFor(int i, int choice, int &s, int &e) {
    if (choice == 0) { s = As[i]; e = Ae[i]; }
    else { s = Bs[i]; e = Be[i]; }
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        int i, j;
        for (i = 0; i < n; ++i) {
            int sh, sm, th, tm, d;
            scanf(" %d:%d %d:%d %d", &sh, &sm, &th, &tm, &d);
            Sh[i] = sh; Sm[i] = sm; Th[i] = th; Tm[i] = tm; Dd[i] = d;
            int S = sh * 60 + sm;
            int Tt = th * 60 + tm;
            As[i] = S; Ae[i] = S + d;
            Bs[i] = Tt - d; Be[i] = Tt;
        }

        int V = 2 * n;
        ecnt = 0;
        for (i = 0; i < V; ++i) {
            head[i] = -1; dfn[i] = 0; low[i] = 0; comp[i] = 0; onStk[i] = false;
        }

        for (i = 0; i < n; ++i) {
            for (j = i + 1; j < n; ++j) {
                int a, b;
                for (a = 0; a < 2; ++a) {
                    for (b = 0; b < 2; ++b) {
                        int is, ie, js, je;
                        intervalFor(i, a, is, ie);
                        intervalFor(j, b, js, je);
                        if (is < je && js < ie) {
                            /* choice a of i conflicts with choice b of j:
                               (x_i == a) -> (x_j == 1-b), (x_j == b) -> (x_i == 1-a) */
                            int litIa = (a == 0) ? T(i) : F(i);
                            int litIa_neg = (a == 0) ? F(i) : T(i);
                            int litJb = (b == 0) ? T(j) : F(j);
                            int litJb_neg = (b == 0) ? F(j) : T(j);
                            addEdge(litIa, litJb_neg);
                            addEdge(litJb, litIa_neg);
                        }
                    }
                }
            }
        }

        idx_ = 0; ncomp = 0; stkTop = 0;
        for (i = 0; i < V; ++i) if (dfn[i] == 0) tarjan(i);

        bool ok = true;
        for (i = 0; i < n; ++i) if (comp[T(i)] == comp[F(i)]) { ok = false; break; }

        if (!ok) {
            printf("NO\n");
        } else {
            printf("YES\n");
            for (i = 0; i < n; ++i) {
                bool choiceStart = comp[T(i)] < comp[F(i)];
                int s, e;
                if (choiceStart) { s = As[i]; e = Ae[i]; }
                else { s = Bs[i]; e = Be[i]; }
                printf("%02d:%02d %02d:%02d\n", s / 60, s % 60, e / 60, e % 60);
            }
        }
    }
    return 0;
}
