// POJ 2117 - Electricity
// Model: claude-opus-5
//
// Given an undirected graph (P <= 10000 vertices, several instances until "0 0"),
// remove exactly one vertex and maximise the number of connected components that
// remain, not counting the removed vertex itself.
//
// Approach: one iterative Tarjan DFS per instance.
//   Let comp = number of connected components of the whole graph.  Removing v
//   leaves the comp-1 components that do not contain v untouched, and breaks v's
//   own component into f(v) pieces:
//     f(root of a DFS tree) = number of its DFS children
//                             (every child subtree of the root separates, since
//                              disc[root] is the minimum in that tree)
//     f(other v)            = 1 + #{DFS children c : low[c] >= disc[v]}
//                             (the "1" is the part still holding v's parent)
//   Answer = max over v of (comp - 1 + f(v)).
//
// The DFS is an explicit stack, not recursion: a path on 10000 vertices would be
// 10000 frames deep.
//
// Pitfalls this handles (both are on the discuss board):
//   "1 0" -> 0   the lone vertex is removed and nothing is left.
//   "3 0" -> 2   isolated vertices are connected components in their own right
//                (a single vertex is a connected component, though not a
//                 biconnected one), so comp counts them and f(v) = 0 for them.
// Note the answer is the count of *connected* components, not biconnected ones.
//
// Adjacency is a CSR of edge codes rather than of endpoints: slot 2*i sits in
// eu[i]'s list and means "cross edge i to ev[i]", slot 2*i+1 sits in ev[i]'s list
// and means "cross edge i to eu[i]".  That keeps the edge id available for the
// parent-edge skip (correct even if the data ever repeated an edge, which the
// statement forbids) while storing 16 bytes per edge instead of 24.
//
// C has no stated upper bound; input is read with a buffered reader and the
// per-instance vectors are reused across instances.

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

static const int BUFSZ = 1 << 16;
static char ibuf[BUFSZ];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

int main() {
    vector<int> eu, ev;          /* endpoints of edge i */
    vector<int> start_, it_, adj;
    vector<int> disc, low, cutcnt, parentEdge, stk;
    vector<char> isroot;

    int P, C;
    while (readInt(P) && readInt(C)) {
        if (P == 0 && C == 0) break;

        eu.clear(); ev.clear();
        for (int i = 0; i < C; i++) {
            int a, b;
            readInt(a); readInt(b);
            eu.push_back(a);
            ev.push_back(b);
        }

        /* CSR bucket boundaries */
        start_.assign(P + 1, 0);
        for (int i = 0; i < C; i++) { start_[eu[i]]++; start_[ev[i]]++; }
        int run = 0;
        for (int v = 0; v < P; v++) { int d = start_[v]; start_[v] = run; run += d; }
        start_[P] = run;

        it_.assign(start_.begin(), start_.end() - 1);
        adj.assign(run, 0);
        for (int i = 0; i < C; i++) {
            adj[it_[eu[i]]++] = 2 * i;
            adj[it_[ev[i]]++] = 2 * i + 1;
        }

        disc.assign(P, 0);
        low.assign(P, 0);
        cutcnt.assign(P, 0);
        isroot.assign(P, 0);
        parentEdge.assign(P, -1);
        stk.clear();
        for (int v = 0; v < P; v++) it_[v] = start_[v];

        int timer = 0;
        int comp = 0;

        for (int r = 0; r < P; r++) {
            if (disc[r]) continue;
            comp++;
            isroot[r] = 1;
            disc[r] = low[r] = ++timer;
            parentEdge[r] = -1;
            stk.push_back(r);
            while (!stk.empty()) {
                int v = stk.back();
                if (it_[v] < start_[v + 1]) {
                    int code = adj[it_[v]++];
                    int e = code >> 1;
                    if (e == parentEdge[v]) continue;
                    int u = (code & 1) ? eu[e] : ev[e];
                    if (disc[u]) {
                        if (disc[u] < low[v]) low[v] = disc[u];
                    } else {
                        parentEdge[u] = e;
                        disc[u] = low[u] = ++timer;
                        stk.push_back(u);
                    }
                } else {
                    stk.pop_back();
                    if (!stk.empty()) {
                        int p = stk.back();              /* v's DFS parent */
                        if (low[v] < low[p]) low[p] = low[v];
                        if (low[v] >= disc[p]) cutcnt[p]++;
                    }
                }
            }
        }

        int best = 0;
        for (int v = 0; v < P; v++) {
            int total = comp - 1 + cutcnt[v] + (isroot[v] ? 0 : 1);
            if (total > best) best = total;
        }
        printf("%d\n", best);
    }
    return 0;
}
