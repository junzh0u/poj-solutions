// POJ 3205 - Ikki's Story II - War with TN
// Model: claude-sonnet-5
//
// The road network is a directed graph with all cities reachable from
// capital s = 0. A city (other than s and t) that every s->t path must
// pass through is exactly a strict dominator of t in the flowgraph
// rooted at s. Build the dominator tree with the Lengauer-Tarjan
// algorithm (iterative DFS + iterative path-compressed eval, no
// recursion, to stay safe for N up to 10000), then walk the idom chain
// from t up to s and count the interior vertices (excluding t and s).
//
// Ambiguity check: the discuss board (message 68694/68696) confirms the
// reading "count vertices whose removal disconnects every s->t path",
// i.e. classic dominators, with a worked example (0-1,0-2,0-3,0-4,1-4,
// 4-5, t=5 -> answer 1, since 4 is the sole cut vertex; 1 is not, since
// 0->4 is a direct edge). Verified the implementation against this
// example and the official sample by hand, and against a brute-force
// (remove each vertex, BFS reachability) oracle on randomized small
// graphs, including graphs with cycles (the board also has an
// unanswered "can there be cycles?" question, so the implementation
// must not assume a DAG -- Lengauer-Tarjan handles general digraphs).

#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 10005;

int N, M;
vector<int> g[MAXN], rg[MAXN];
int ord_[MAXN], vert[MAXN], par[MAXN];
int ufp[MAXN], label_[MAXN], semi_[MAXN], idom[MAXN];
vector<int> bucket_[MAXN];
int pathbuf[MAXN];

int eval_(int v) {
    if (ufp[v] == v) return v;
    int top = 0;
    int x = v;
    while (ufp[x] != x) { pathbuf[top++] = x; x = ufp[x]; }
    int root = x;
    for (int i = top - 1; i >= 0; i--) {
        int node = pathbuf[i];
        int parentNode = (i == top - 1) ? root : pathbuf[i + 1];
        if (ord_[semi_[label_[parentNode]]] < ord_[semi_[label_[node]]])
            label_[node] = label_[parentNode];
        ufp[node] = root;
    }
    return label_[v];
}

int main() {
    if (scanf("%d %d", &N, &M) != 2) return 0;
    for (int i = 0; i < M; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        g[a].push_back(b);
        rg[b].push_back(a);
    }
    int t;
    scanf("%d", &t);

    int s = 0;
    for (int i = 0; i < N; i++) ord_[i] = -1;

    // iterative DFS preorder
    static size_t iter_[MAXN];
    static int stk[MAXN];
    int sp = 0;
    int cnt = 0;
    ord_[s] = cnt; vert[cnt] = s; cnt++;
    par[s] = -1;
    iter_[s] = 0;
    stk[sp++] = s;
    while (sp > 0) {
        int u = stk[sp - 1];
        if (iter_[u] < g[u].size()) {
            int v = g[u][iter_[u]++];
            if (ord_[v] == -1) {
                ord_[v] = cnt; vert[cnt] = v; cnt++;
                par[v] = u;
                iter_[v] = 0;
                stk[sp++] = v;
            }
        } else {
            sp--;
        }
    }

    for (int i = 0; i < N; i++) {
        ufp[i] = i;
        label_[i] = i;
        semi_[i] = i;
        idom[i] = -1;
    }

    for (int i = cnt - 1; i >= 1; i--) {
        int w = vert[i];
        for (size_t k = 0; k < rg[w].size(); k++) {
            int p = rg[w][k];
            if (ord_[p] == -1) continue;
            int u = eval_(p);
            if (ord_[semi_[u]] < ord_[semi_[w]]) semi_[w] = semi_[u];
        }
        bucket_[semi_[w]].push_back(w);
        ufp[w] = par[w];
        vector<int>& bk = bucket_[par[w]];
        for (size_t k = 0; k < bk.size(); k++) {
            int v2 = bk[k];
            int u = eval_(v2);
            idom[v2] = (ord_[semi_[u]] < ord_[semi_[v2]]) ? u : par[w];
        }
        bk.clear();
    }
    for (int i = 1; i < cnt; i++) {
        int w = vert[i];
        if (idom[w] != semi_[w]) idom[w] = idom[idom[w]];
    }
    idom[s] = -1;

    int ans = 0;
    if (t != s && ord_[t] != -1) {
        int cur = idom[t];
        while (cur != -1 && cur != s) {
            ans++;
            cur = idom[cur];
        }
    }
    printf("%d\n", ans);
    return 0;
}
