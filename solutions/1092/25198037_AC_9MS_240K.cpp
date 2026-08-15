// POJ 1092 - Farmland
// Model: claude-sonnet-5
//
// Approach: this is the classic "trace faces of a plane graph from its
// rotation system" problem. Each vertex's neighbours are sorted by true
// polar angle (using exact integer cross-product comparisons, no floating
// point) around that vertex from its (x,y) coordinates -- the input's own
// neighbour listing order is NOT trusted, since the geometrically correct
// rotation is what actually determines the embedding's faces.
//
// For every directed edge (u,v) that hasn't been used yet, trace the face
// to its left: standing at v having arrived from u, look up u's position in
// v's angle-sorted neighbour list and step to the *next* entry (cyclically)
// to get the outgoing neighbour w; continue with edge (v,w) until the walk
// returns to the starting edge (u,v). This is the standard DCEL "next edge"
// rule and partitions all 2*E directed edges into exactly F faces
// (Euler's formula), one trace per face.
//
// A traced face is a genuine "proper farming region" iff:
//   1. all vertices visited in its walk are distinct (a simple cycle) --
//      a repeated vertex means some edge or sub-structure (e.g. a bridge
//      like the v6-v8 / v11-v10 edges in the statement's own figure) juts
//      into the region, which the problem explicitly disqualifies; and
//   2. it is not the outer face.
// Property 2 is decided by the sign of the polygon's shoelace area computed
// from the real coordinates: with the "next = successor in CCW-sorted
// order" rule, every bounded face's trace comes out with negative shoelace
// area and the single unbounded outer face comes out positive -- this sign
// was pinned down empirically against the statement's own worked example
// (regions <v1,v9,v8,v7> and <v2,v3,v4,v5> both traced with negative area,
// the outer face positive) rather than trusted from a hand derivation, and
// it plays the same distinguishing role as the (n-2)*pi interior-angle-sum
// test an accepted solution posted on the discuss board uses for the same
// purpose (message 351099), just via exact integer arithmetic instead of
// floating point angles.
//
// The discuss board (message 346582) warns that a chord edge strictly
// inside an otherwise-simple face boundary (e.g. an edge 2-4 across a
// simple quadrilateral 1-2-3-4) also disqualifies the face, even though no
// *vertex* lies inside it. This needs no special-casing: since that chord
// is a real graph edge, it is already part of vertex 2's and vertex 4's
// sorted rotations, so the "next edge" rule never traces 1-2-3-4 as a
// single face to begin with -- it correctly splits it into two smaller
// faces along the chord.
//
// No statement ambiguity beyond the above (confirmed by the board);
// M<10, N<200 so plain O(N^2) bookkeeping is ample.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static int X[205], Y[205];
static int idxOf[205][205];      // idxOf[v][nbr] = index of nbr within sortedAdj[v]
static bool adjMat[205][205];
static bool visited[205][205];
static vector<int> sortedAdj[205];

struct AngleCmp {
    int cx, cy;
    AngleCmp(int cx_, int cy_) : cx(cx_), cy(cy_) {}
    static int half(int dx, int dy) {
        // 0 = upper half-plane incl. +x axis, 1 = lower half-plane incl. -x axis
        if (dy > 0 || (dy == 0 && dx > 0)) return 0;
        return 1;
    }
    bool operator()(int a, int b) const {
        int dxa = X[a] - cx, dya = Y[a] - cy;
        int dxb = X[b] - cx, dyb = Y[b] - cy;
        int ha = half(dxa, dya), hb = half(dxb, dyb);
        if (ha != hb) return ha < hb;
        int cross = dxa * dyb - dxb * dya;
        return cross > 0;
    }
};

int main() {
    int M;
    if (scanf("%d", &M) != 1) return 0;
    for (int tc = 0; tc < M; tc++) {
        int N;
        scanf("%d", &N);
        for (int v = 1; v <= N; v++) {
            sortedAdj[v].clear();
            for (int w = 1; w <= N; w++) { adjMat[v][w] = false; visited[v][w] = false; }
        }
        for (int t = 0; t < N; t++) {
            int i, x, y, d;
            scanf("%d %d %d %d", &i, &x, &y, &d);
            X[i] = x; Y[i] = y;
            sortedAdj[i].resize(d);
            for (int k = 0; k < d; k++) {
                int a;
                scanf("%d", &a);
                sortedAdj[i][k] = a;
                adjMat[i][a] = true;
            }
        }
        for (int v = 1; v <= N; v++) {
            sort(sortedAdj[v].begin(), sortedAdj[v].end(), AngleCmp(X[v], Y[v]));
            for (size_t k = 0; k < sortedAdj[v].size(); k++) idxOf[v][sortedAdj[v][k]] = (int)k;
        }

        vector<int> sizeCount(N + 2, 0);
        vector<int> path;
        for (int u = 1; u <= N; u++) {
            for (int v = 1; v <= N; v++) {
                if (!adjMat[u][v] || visited[u][v]) continue;
                path.clear();
                int curU = u, curV = v;
                while (true) {
                    path.push_back(curU);
                    int idx = idxOf[curV][curU];
                    int deg = (int)sortedAdj[curV].size();
                    int w = sortedAdj[curV][(idx + 1) % deg];
                    visited[curU][curV] = true;
                    if (curV == u && w == v) break;
                    curU = curV;
                    curV = w;
                }
                int L = (int)path.size();
                bool simple = true;
                {
                    static bool seen[205];
                    for (int k = 0; k <= N; k++) seen[k] = false;
                    for (int k = 0; k < L; k++) {
                        if (seen[path[k]]) { simple = false; break; }
                        seen[path[k]] = true;
                    }
                }
                if (simple) {
                    int area2 = 0;
                    for (int k = 0; k < L; k++) {
                        int a = path[k], b = path[(k + 1) % L];
                        area2 += X[a] * Y[b] - X[b] * Y[a];
                    }
                    if (area2 < 0 && L <= N) sizeCount[L]++;
                }
            }
        }

        int k;
        scanf("%d", &k);
        int ans = (k >= 0 && k <= N + 1) ? sizeCount[k] : 0;
        printf("%d\n", ans);
    }
    return 0;
}
