// POJ 2296 - Map Labeler
// Model: claude-sonnet-5
// Approach: binary search on the integer label side length L, checked with
// 2-SAT. Each city gets a boolean orientation (label attached above/below,
// i.e. the city sits at the midpoint of the label's bottom or top edge). For
// a fixed L, two labels' squares share the same horizontal extent regardless
// of orientation, so a pair of cities i,j can only conflict when
// |xi - xj| < L (strict). Given that, comparing the two possible vertical
// extents [y, y+L] (up) and [y-L, y] (down) for each of the 4 orientation
// combinations yields, after canceling the common half-width, purely integer
// strict-inequality conditions (no fractions needed):
//   up/up forbidden   iff |yi - yj| < L
//   down/down forbidden iff |yi - yj| < L
//   (i up, j down) forbidden iff 0 < yj - yi < 2L
//   (i down, j up) forbidden iff 0 < yi - yj < 2L
// "Sharing an edge" is explicitly allowed, so all overlap tests are strict
// (touching boundaries are fine); this naturally also covers the yi == yj
// case the board flags as a common special-case bug (the discuss board
// thread "y-difference of 0 needs special handling") -- with yi == yj only
// up/up and down/down are forbidden, forcing the two labels to differ, which
// falls out of the inequalities above without any extra case.
// Each forbidden combination becomes a 2-SAT clause; feasibility is
// monotonic non-increasing in L (larger L only adds constraints), so binary
// search for the largest feasible L. Upper bound 40000 follows a board post
// reporting that 10000 was insufficient and 20000 was required for some
// data set; 40000 is double that margin.
// 2-SAT solved via Kosaraju's two-pass DFS/SCC (m <= 100, trivially small).

#include <cstdio>
#include <vector>
using namespace std;

int m;
int X[105], Y[105];

struct TwoSAT {
    int n;
    vector<vector<int> > adj, radj;
    vector<int> order_, comp;
    vector<char> visited;

    void init(int n_) {
        n = n_;
        adj.assign(2 * n, vector<int>());
        radj.assign(2 * n, vector<int>());
    }
    void addImpl(int a, int b) {
        adj[a].push_back(b);
        radj[b].push_back(a);
    }
    // forbid combination (var a == av) AND (var b == bv):
    // add clause (a==!av) OR (b==!bv)
    void forbid(int a, bool av, int b, bool bv) {
        int notA = 2 * a + (av ? 1 : 0); // node meaning a == !av
        int A = 2 * a + (av ? 0 : 1);    // node meaning a == av
        int notB = 2 * b + (bv ? 1 : 0);
        int B = 2 * b + (bv ? 0 : 1);
        addImpl(A, notB);
        addImpl(B, notA);
    }
    void dfs1(int u) {
        visited[u] = 1;
        for (size_t k = 0; k < adj[u].size(); ++k) {
            int v = adj[u][k];
            if (!visited[v]) dfs1(v);
        }
        order_.push_back(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c;
        for (size_t k = 0; k < radj[u].size(); ++k) {
            int v = radj[u][k];
            if (comp[v] == -1) dfs2(v, c);
        }
    }
    bool solve() {
        int N = 2 * n;
        visited.assign(N, 0);
        order_.clear();
        for (int i = 0; i < N; ++i)
            if (!visited[i]) dfs1(i);
        comp.assign(N, -1);
        int c = 0;
        for (int i = N - 1; i >= 0; --i) {
            int u = order_[i];
            if (comp[u] == -1) dfs2(u, c++);
        }
        for (int i = 0; i < n; ++i)
            if (comp[2 * i] == comp[2 * i + 1]) return false;
        return true;
    }
};

bool feasible(int L) {
    if (L <= 0) return true;
    TwoSAT ts;
    ts.init(m);
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            int dx = X[i] - X[j];
            if (dx < 0) dx = -dx;
            if (dx >= L) continue; // no horizontal overlap possible
            int dy = Y[i] - Y[j];
            if (dy < 0) dy = -dy;
            if (dy < L) {
                ts.forbid(i, true, j, true);   // up/up forbidden
                ts.forbid(i, false, j, false); // down/down forbidden
            }
            int yj_yi = Y[j] - Y[i];
            if (yj_yi > 0 && yj_yi < 2 * L) {
                ts.forbid(i, true, j, false); // (i up, j down) forbidden
            }
            int yi_yj = Y[i] - Y[j];
            if (yi_yj > 0 && yi_yj < 2 * L) {
                ts.forbid(i, false, j, true); // (i down, j up) forbidden
            }
        }
    }
    return ts.solve();
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        scanf("%d", &m);
        for (int i = 0; i < m; ++i) {
            int x, y;
            scanf("%d %d", &x, &y);
            X[i] = x;
            Y[i] = y;
        }
        int lo = 0, hi = 40000, ans = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(mid)) {
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
