// POJ 4000 - National Treasures
// Model: claude-sonnet-5
// Approach: For each artifact cell A with a set bit pointing to critical
// point cell B (in bounds), the constraint is "A becomes a guard OR B
// becomes a guard" (if A stays as an artifact, its critical points must
// already be guarded). If B already holds a museum guard (-1), the
// constraint is trivially satisfied and produces no edge. Every offset in
// the 12-point diagram has (|dr|+|dc|) odd, so it always connects a cell
// to a cell of opposite (row+col) parity -- the constraint graph is
// bipartite. The minimum number of additional guards is exactly the
// minimum vertex cover of this bipartite graph, which by Konig's theorem
// equals the maximum matching, computed here with Hopcroft-Karp.
// No statement ambiguity beyond confirming the bipartite structure, which
// matches the discuss board's own explanation (coordinate transform
// (x+y,x-y) proof that every cycle has even length).
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

static const int MAXN = 2510;
static const int INF = 1 << 30;

int n;
vector<int> adj[MAXN];
int matchL[MAXN], matchR[MAXN], dist_[MAXN];
bool isLeft[MAXN];

bool bfs() {
    queue<int> q;
    for (int u = 0; u < n; u++) {
        if (isLeft[u] && matchL[u] == -1) {
            dist_[u] = 0;
            q.push(u);
        } else {
            dist_[u] = INF;
        }
    }
    bool found = false;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            int w = matchR[v];
            if (w == -1) {
                found = true;
            } else if (dist_[w] == INF) {
                dist_[w] = dist_[u] + 1;
                q.push(w);
            }
        }
    }
    return found;
}

bool dfs(int u) {
    for (size_t i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        int w = matchR[v];
        if (w == -1 || (dist_[w] == dist_[u] + 1 && dfs(w))) {
            matchL[u] = v;
            matchR[v] = u;
            return true;
        }
    }
    dist_[u] = INF;
    return false;
}

int hopcroftKarp() {
    for (int i = 0; i < n; i++) { matchL[i] = -1; matchR[i] = -1; }
    int result = 0;
    while (bfs()) {
        for (int u = 0; u < n; u++) {
            if (isLeft[u] && matchL[u] == -1) {
                if (dfs(u)) result++;
            }
        }
    }
    return result;
}

int main() {
    static int drOff[12] = {-1,-2,-2,-1,1,2,2,1,-1,0,1,0};
    static int dcOff[12] = {-2,-1,1,2,2,1,-1,-2,0,1,0,-1};

    int R, C;
    int caseNum = 0;
    static int grid[2510];
    while (scanf("%d %d", &R, &C) == 2) {
        if (R == 0 && C == 0) break;
        caseNum++;
        n = R * C;
        for (int i = 0; i < n; i++) {
            scanf("%d", &grid[i]);
        }
        for (int i = 0; i < n; i++) adj[i].clear();
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                int id = r * C + c;
                int color = (r + c) & 1;
                isLeft[id] = (color == 0);
                int t = grid[id];
                if (t < 0) continue; // guard cell, no artifact, no constraints from it
                for (int b = 0; b < 12; b++) {
                    if (!((t >> b) & 1)) continue;
                    int nr = r + drOff[b];
                    int nc = c + dcOff[b];
                    if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue; // outside: secure
                    int nid = nr * C + nc;
                    if (grid[nid] < 0) continue; // already guarded: satisfied
                    // constraint edge between id and nid; add to left->right adjacency
                    if (color == 0) {
                        adj[id].push_back(nid);
                    } else {
                        adj[nid].push_back(id);
                    }
                }
            }
        }
        int ans = hopcroftKarp();
        printf("%d. %d\n", caseNum, ans);
    }
    return 0;
}
