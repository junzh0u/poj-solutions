// POJ 2567 - Code the Tree
// Model: claude-opus-5
//
// Parse the parenthesised tree notation "(N T T ...)" with a stack: on '(' read
// the vertex label, join it to the label on top of the stack (that is the edge
// to its parent) and push it; on ')' pop.  When the stack empties, one whole
// tree has been read, so this parses per-tree rather than per-line and is
// immune to line-end oddities, blank lines and CRs.  Input runs to EOF.
//
// Then simulate the statement literally: repeatedly take the smallest-numbered
// leaf (degree 1), print the label of its unique remaining neighbour, and delete
// it, until a single vertex is left -- n-1 numbers, i.e. the standard Prufer
// code (n-2 entries) followed by the last vertex n.  n <= 50, so the O(n^2)
// rescan for the minimum leaf is free.
//
// Pitfall (confirmed on the discuss board, and the sample never reaches it):
// a one-vertex tree "(1)" has an empty code and must still produce an empty
// output line -- omitting the newline gives Presentation Error.
//
// Labels are read with x = x*10 + digit, so two-digit vertex numbers (n can be
// 50) parse correctly.

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

static void solve(const vector<int>& verts,
                  const vector<pair<int, int> >& edges,
                  int maxv) {
    int n = (int)verts.size();
    if (n <= 1) {           /* empty Prufer code, but still a line */
        putchar('\n');
        return;
    }

    vector<vector<int> > adj(maxv + 1);
    vector<int> deg(maxv + 1, 0);
    vector<char> alive(maxv + 1, 0);

    for (int i = 0; i < n; i++) alive[verts[i]] = 1;
    for (size_t i = 0; i < edges.size(); i++) {
        int a = edges[i].first, b = edges[i].second;
        adj[a].push_back(b);
        adj[b].push_back(a);
        deg[a]++;
        deg[b]++;
    }

    bool first = true;
    for (int step = 0; step < n - 1; step++) {
        int leaf = -1;
        for (int v = 1; v <= maxv; v++) {
            if (alive[v] && deg[v] == 1) { leaf = v; break; }
        }
        if (leaf < 0) break;                 /* cannot happen on a tree */
        int nb = -1;
        for (size_t i = 0; i < adj[leaf].size(); i++) {
            if (alive[adj[leaf][i]]) { nb = adj[leaf][i]; break; }
        }
        alive[leaf] = 0;
        deg[leaf] = 0;
        if (nb >= 0) deg[nb]--;
        if (!first) putchar(' ');
        printf("%d", nb);
        first = false;
    }
    putchar('\n');
}

int main() {
    vector<int> stk, verts;
    vector<pair<int, int> > edges;
    int maxv = 0;
    bool started = false;
    int c;

    while ((c = getchar()) != EOF) {
        if (c == '(') {
            int d = getchar();
            while (d == ' ' || d == '\t' || d == '\r' || d == '\n') d = getchar();
            int x = 0;
            bool got = false;
            while (d != EOF && d >= '0' && d <= '9') {
                x = x * 10 + (d - '0');
                got = true;
                d = getchar();
            }
            if (d != EOF) ungetc(d, stdin);
            if (!got) continue;
            if (!stk.empty()) edges.push_back(make_pair(stk.back(), x));
            stk.push_back(x);
            verts.push_back(x);
            if (x > maxv) maxv = x;
            started = true;
        } else if (c == ')') {
            if (!stk.empty()) stk.pop_back();
            if (stk.empty() && started) {
                solve(verts, edges, maxv);
                verts.clear();
                edges.clear();
                maxv = 0;
                started = false;
            }
        }
    }
    return 0;
}
