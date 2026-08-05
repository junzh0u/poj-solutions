// POJ 3522 - Slim Span
//
// The slimness of a spanning tree is (heaviest edge - lightest edge); we want
// the smallest slimness over all spanning trees, or -1 if the graph has none.
//
// Sort the edges by weight.  Fix the index i of the lightest edge the tree is
// allowed to use and run Kruskal over edges i..m-1 only: the first spanning
// tree it completes is the one whose heaviest edge is as small as possible for
// that lower bound, so it is the best candidate starting at i.  Edge i itself
// is always taken (at that point every vertex is still its own component and
// there are no self-loops), so the tree's lightest edge really is w[i] and the
// candidate slimness is (last edge added) - w[i].  Answer = best over all i,
// -1 if no start ever spans.  That is O(m^2 a(n)) with m <= 4950 - trivially
// inside the 5s limit.
//
// Two prunings, both exact:
//   * skip a start i whose weight equals w[i-1] - same lower bound over a
//     strictly smaller edge set, so it can never beat start i-1.  Without it,
//     an input of many max-size all-equal-weight graphs is ~60x slower.
//   * stop once slimness 0 is reached.
//
// The statement guarantees a simple graph, but self-loops are dropped on input
// anyway since one at position i would break the "edge i is always taken"
// invariant.  n = 1 never occurs (2 <= n <= 100) but is answered 0 for safety.
// m = 0 and disconnected graphs are real cases in the sample and give -1.

#include <cstdio>
#include <algorithm>

using namespace std;

struct Edge {
    int a, b, w;
};

static bool byW(const Edge &x, const Edge &y) { return x.w < y.w; }

static int par[105];

static int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

int main() {
    int n, m;
    static Edge e[5000];
    while (scanf("%d %d", &n, &m) == 2 && (n != 0 || m != 0)) {
        int i, j, k;
        int cm = 0;
        for (i = 0; i < m; ++i) {
            int a, b, w;
            scanf("%d %d %d", &a, &b, &w);
            /* the statement promises no self-loops; drop them anyway so the
               "edge i is always the tree's lightest edge" invariant holds */
            if (a == b) continue;
            e[cm].a = a;
            e[cm].b = b;
            e[cm].w = w;
            ++cm;
        }
        m = cm;
        sort(e, e + m, byW);

        int best = -1;
        if (n == 1) {
            best = 0;
        } else {
            for (i = 0; i < m; ++i) {
                if (m - i < n - 1) break;
                /* starting one past an equal-weight edge keeps the same lower
                   bound over a smaller edge set, so it can never do better */
                if (i > 0 && e[i].w == e[i - 1].w) continue;
                for (k = 1; k <= n; ++k) par[k] = k;
                int cnt = 0, maxw = 0;
                for (j = i; j < m; ++j) {
                    int ra = find(e[j].a), rb = find(e[j].b);
                    if (ra == rb) continue;
                    par[ra] = rb;
                    maxw = e[j].w;
                    if (++cnt == n - 1) break;
                }
                if (cnt == n - 1) {
                    int d = maxw - e[i].w;
                    if (best < 0 || d < best) best = d;
                    if (best == 0) break;
                }
            }
        }
        printf("%d\n", best);
    }
    return 0;
}
