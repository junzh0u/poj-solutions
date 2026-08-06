// POJ 2723 - Get Luffy Out
// Model: claude-sonnet-5
//
// Approach:
// Each of the N pairs of keys is a boolean choice: pick the first key of the
// pair (variable true) or the second key (variable false) -- exactly one of
// the two keys in a pair can be brought along. A door with locks (u, v) can
// be opened iff the key u OR the key v was chosen -- a classic 2-SAT clause
// over the N pair-variables. Since the M doors must be passed in the given
// order and Ratish stops at the first door he cannot open, the answer is the
// largest prefix length k such that the first k door-clauses are all
// simultaneously satisfiable. Feasibility is monotone in k (more clauses
// only add constraints), so binary search on k and, for each candidate,
// build the 2-SAT implication graph for the first k clauses and test
// satisfiability with Tarjan SCC (a pair variable's true/false literals must
// land in different SCCs for every pair).
//
// No statement ambiguities beyond the usual: N and M bounds are given as
// 2^10 and 2^11 in the HTML (<sup> tags), i.e. N <= 1024, M <= 2048, so the
// 2-SAT graph has at most 2*1024 = 2048 nodes and, per binary-search probe,
// at most 2*2048 = 4096 edges.

#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int MAXV = 2100;   // 2*N nodes, N <= 1024
const int MAXM = 2100;   // M <= 2048 doors
const int MAXK = 2100;   // 2*N keys

int N, M;
int keyPair[2 * MAXK];   // pair index for a key
bool keyFirst[2 * MAXK]; // true if key is the "first" key of its pair
int litOfKey[2 * MAXK];  // literal node id for a key (0..2N-1)
int doorU[MAXM], doorV[MAXM];

vector<int> adj[MAXV];
int dfn[MAXV], low[MAXV], sccId[MAXV];
bool onStack[MAXV];
int stk[MAXV], stkTop;
int dfsClock, sccCount;
int nodeCount;

void tarjan(int u) {
    dfn[u] = low[u] = ++dfsClock;
    stk[++stkTop] = u;
    onStack[u] = true;
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (!dfn[v]) {
            tarjan(v);
            if (low[v] < low[u]) low[u] = low[v];
        } else if (onStack[v]) {
            if (dfn[v] < low[u]) low[u] = dfn[v];
        }
    }
    if (low[u] == dfn[u]) {
        ++sccCount;
        for (;;) {
            int v = stk[stkTop--];
            onStack[v] = false;
            sccId[v] = sccCount;
            if (v == u) break;
        }
    }
}

// Check whether the first k door clauses are simultaneously satisfiable.
bool check(int k) {
    for (int i = 0; i < nodeCount; ++i) {
        adj[i].clear();
        dfn[i] = 0;
        low[i] = 0;
        onStack[i] = false;
        sccId[i] = 0;
    }
    for (int i = 0; i < k; ++i) {
        int a = litOfKey[doorU[i]];
        int b = litOfKey[doorV[i]];
        // clause (a OR b): NOT a -> b, NOT b -> a
        adj[a ^ 1].push_back(b);
        adj[b ^ 1].push_back(a);
    }
    dfsClock = 0;
    sccCount = 0;
    stkTop = -1;
    for (int i = 0; i < nodeCount; ++i) {
        if (!dfn[i]) tarjan(i);
    }
    for (int i = 0; i < N; ++i) {
        if (sccId[2 * i] == sccId[2 * i + 1]) return false;
    }
    return true;
}

int main() {
    while (scanf("%d %d", &N, &M) == 2) {
        if (N == 0 && M == 0) break;
        nodeCount = 2 * N;
        for (int i = 0; i < N; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            keyPair[a] = i;
            keyFirst[a] = true;
            keyPair[b] = i;
            keyFirst[b] = false;
        }
        for (int k = 0; k < 2 * N; ++k) {
            litOfKey[k] = 2 * keyPair[k] + (keyFirst[k] ? 0 : 1);
        }
        for (int i = 0; i < M; ++i) {
            scanf("%d %d", &doorU[i], &doorV[i]);
        }

        int lo = 0, hi = M, ans = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (check(mid)) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
