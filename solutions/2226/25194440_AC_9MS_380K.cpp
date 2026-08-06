// POJ 2226 - Muddy Fields
//
// Cover every muddy ('*') cell with 1-wide boards of any length, aligned to a
// row or a column, never touching grass ('.'); boards may overlap. Minimise
// the number of boards.
//
// A board is never worth shortening: extending it to the whole maximal run of
// mud it lies in stays legal (a maximal run has grass or the border at both
// ends) and covers at least as much. So the only candidate boards are the
// maximal horizontal runs and the maximal vertical runs of mud, and each muddy
// cell must be covered by the horizontal run through it or the vertical run
// through it.
//
// Build a bipartite graph: horizontal runs on the left, vertical runs on the
// right, one edge per muddy cell joining the two runs that meet there. Picking
// a set of boards that covers all mud is exactly picking a vertex cover of
// this graph, so the answer is the minimum vertex cover, which by Koenig's
// theorem equals the maximum matching. Kuhn's augmenting-path algorithm gets
// it; with R, C <= 50 there are at most 1250 runs per side and 2500 edges, so
// O(V*E) is trivially fast (9MS here).
//
// Statement notes: boards may not cover grass at all (not even to bridge two
// muddy cells), which is what makes "maximal run" the right unit; overlap
// between boards is explicitly allowed, so the two orientations are chosen
// independently per cell and no extra cost is paid where boards cross.

#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int MAXR = 55;
const int MAXN = MAXR * MAXR / 2 + 10; /* max runs per orientation */

int R, C;
char g[MAXR][MAXR];
int hid[MAXR][MAXR]; /* horizontal run id of cell */
int vid[MAXR][MAXR]; /* vertical run id of cell */

vector<int> adj[MAXN];
int matchL[MAXN], matchR[MAXN];
bool used[MAXN];

bool tryAug(int u) {
    for (size_t k = 0; k < adj[u].size(); ++k) {
        int v = adj[u][k];
        if (used[v]) continue;
        used[v] = true;
        if (matchR[v] == -1 || tryAug(matchR[v])) {
            matchR[v] = u;
            matchL[u] = v;
            return true;
        }
    }
    return false;
}

int main() {
    if (scanf("%d %d", &R, &C) != 2) return 0;
    for (int i = 0; i < R; ++i) scanf("%s", g[i]);

    int nh = 0;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (g[i][j] == '*') {
                if (j == 0 || g[i][j - 1] != '*') ++nh;
                hid[i][j] = nh - 1;
            } else {
                hid[i][j] = -1;
            }
        }
    }
    int nv = 0;
    for (int j = 0; j < C; ++j) {
        for (int i = 0; i < R; ++i) {
            if (g[i][j] == '*') {
                if (i == 0 || g[i - 1][j] != '*') ++nv;
                vid[i][j] = nv - 1;
            } else {
                vid[i][j] = -1;
            }
        }
    }

    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            if (g[i][j] == '*') adj[hid[i][j]].push_back(vid[i][j]);

    for (int i = 0; i < nh; ++i) matchL[i] = -1;
    for (int i = 0; i < nv; ++i) matchR[i] = -1;

    int res = 0;
    for (int i = 0; i < nh; ++i) {
        memset(used, 0, sizeof(bool) * nv);
        if (tryAug(i)) ++res;
    }
    printf("%d\n", res);
    return 0;
}
