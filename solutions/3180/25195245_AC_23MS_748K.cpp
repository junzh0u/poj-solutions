// POJ 3180 - The Cow Prom
// Model: claude-opus-5
//
// Each rope is a directed edge A->B.  A group can perform the Round Dance iff
// every cow in it can pull every other cow both ways, i.e. the group is a
// strongly connected component with at least two cows; the answer is the number
// of such components.
//
// Ambiguity: the statement first describes a cow's "group" as everything
// reachable through her ropes (a weakly connected component), which would count
// only WCCs that happen to be strongly connected -- but the "dangling cow"
// remark and the discuss board both settle it as plain SCCs of size >= 2, so a
// component hanging off a strongly connected core does not disqualify the core.
// The edge orientation convention (whether a rope pulls A->B or B->A) is
// irrelevant: reversing every edge leaves the SCCs unchanged.
//
// Tarjan's algorithm, written iteratively -- N is 10,000, so a recursive DFS
// would nest 10,000 frames deep on a path graph.  O(N + M).

#include <cstdio>

const int MAXN = 10005;
const int MAXM = 50005;

int head[MAXN], nxt[MAXM], dest[MAXM];
int dfn[MAXN], low[MAXN], it[MAXN], stk[MAXN], cs[MAXN];
bool onstk[MAXN];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) head[i] = -1;
    for (int e = 0; e < m; ++e) {
        int a, b;
        scanf("%d %d", &a, &b);
        dest[e] = b;
        nxt[e] = head[a];
        head[a] = e;
    }

    int timer = 0, top = 0, ans = 0;
    for (int s = 1; s <= n; ++s) {
        if (dfn[s]) continue;
        int sp = 0;
        cs[0] = s;
        dfn[s] = low[s] = ++timer;
        stk[++top] = s;
        onstk[s] = true;
        it[s] = head[s];
        while (sp >= 0) {
            int u = cs[sp];
            if (it[u] != -1) {
                int e = it[u];
                it[u] = nxt[e];
                int v = dest[e];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++timer;
                    stk[++top] = v;
                    onstk[v] = true;
                    it[v] = head[v];
                    cs[++sp] = v;
                } else if (onstk[v] && dfn[v] < low[u]) {
                    low[u] = dfn[v];
                }
            } else {
                if (low[u] == dfn[u]) {
                    int cnt = 0;
                    for (;;) {
                        int w = stk[top--];
                        onstk[w] = false;
                        ++cnt;
                        if (w == u) break;
                    }
                    if (cnt >= 2) ++ans;
                }
                --sp;
                if (sp >= 0 && low[u] < low[cs[sp]]) low[cs[sp]] = low[u];
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
