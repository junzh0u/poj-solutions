// POJ 1436 - Horizontally Visible Segments
// Model: claude-opus-5
//
// Sweep the vertical segments by increasing x with a "paint the interval"
// segment tree keyed on y, then count triangles in the visibility graph.
//
// Coordinates are DOUBLED and the tree leaves are points 0..16000.  Both halves
// of that matter:
//   * leaves are points, not cells, so two segments touching at a single y
//     (e.g. [0,3] and [3,4]) share leaf 2*3 and are found visible.  The sample
//     needs this: its only triangle uses the pair (0 3 1)/(3 4 2), which meet
//     only at y=3, so answer 1 rules out the "touching does not count" reading.
//   * doubling keeps the gaps between integer y's representable.  With plain
//     integer leaves, blockers [0,1] and [2,3] would erase a segment [0,3] even
//     though the open gap (1,2) is free; the odd leaf 2*1+1 preserves it.
// Every non-empty free set of y is a union of intervals with integer endpoints,
// so it always contains a multiple of 1/2 -- the doubled grid is exact, not an
// approximation.
//
// When segment j is inserted, the tree holds, for each y point, the segment
// with the largest x < x_j covering it; the distinct colours over j's range are
// exactly j's left-visible partners.  Segments sharing an x are queried as a
// group before any of them is painted: they are disjoint, hence never mutually
// visible, and one must not shadow another (x_k strictly between is required to
// block).
//
// The visibility graph is planar, so |E| = O(n) (measured max ~20600 at
// n=8000).  Adjacency is kept as an n x n bitset (8 MB, well under the 64 MB
// limit -- an 8000x8000 char matrix reportedly needs 63 MB) and each edge (u,v)
// contributes popcount(adj[u] & adj[v]) restricted to w > v, so every triangle
// is counted once.  20 worst-case data sets run in 0.12s locally.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 8000;
const int MAXYP = 16001;          /* doubled coordinates: points 0..16000 */
const int TOPY = 16000;
const int WORDS = (MAXN + 31) / 32;

static int col[4 * MAXYP];        /* 0 = empty, -1 = mixed, c>0 = segment c-1 */
static unsigned int adjb[MAXN][WORDS];
static int sy1[MAXN], sy2[MAXN], sx[MAXN];
static int ord[MAXN];
static int stampv[MAXN];
static int foundv[MAXN];
static int nfound, curstamp;
static unsigned char pc16[1 << 16];

struct CmpX {
    bool operator()(int a, int b) const { return sx[a] < sx[b]; }
};

static void tset(int node, int l, int r, int ql, int qr, int c) {
    if (ql <= l && r <= qr) { col[node] = c; return; }
    int mid = (l + r) >> 1;
    if (col[node] != -1) {
        col[node << 1] = col[node];
        col[node << 1 | 1] = col[node];
        col[node] = -1;
    }
    if (ql <= mid) tset(node << 1, l, mid, ql, qr, c);
    if (qr > mid) tset(node << 1 | 1, mid + 1, r, ql, qr, c);
}

static void tquery(int node, int l, int r, int ql, int qr) {
    if (col[node] != -1) {
        int c = col[node];
        if (c != 0 && stampv[c - 1] != curstamp) {
            stampv[c - 1] = curstamp;
            foundv[nfound++] = c - 1;
        }
        return;
    }
    int mid = (l + r) >> 1;
    if (ql <= mid) tquery(node << 1, l, mid, ql, qr);
    if (qr > mid) tquery(node << 1 | 1, mid + 1, r, ql, qr);
}

int main() {
    for (int i = 0; i < (1 << 16); i++) {
        int c = 0, v = i;
        while (v) { c += v & 1; v >>= 1; }
        pc16[i] = (unsigned char)c;
    }
    static const unsigned int DEB = 0x077CB531u;
    static const int debtab[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    int d;
    if (scanf("%d", &d) != 1) return 0;
    while (d--) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d", &sy1[i], &sy2[i], &sx[i]);
            ord[i] = i;
        }
        int nw = (n + 31) / 32;
        for (int i = 0; i < n; i++) memset(adjb[i], 0, nw * sizeof(unsigned int));
        memset(col, 0, sizeof(col));
        memset(stampv, 0, n * sizeof(int));

        sort(ord, ord + n, CmpX());

        /* sweep by x; segments sharing an x are queried as a group before any
           of them is inserted, so same-x segments never shadow each other */
        int k = 0;
        while (k < n) {
            int k2 = k;
            while (k2 < n && sx[ord[k2]] == sx[ord[k]]) k2++;
            for (int t = k; t < k2; t++) {
                int i = ord[t];
                nfound = 0;
                curstamp = i + 1;
                tquery(1, 0, TOPY, 2 * sy1[i], 2 * sy2[i]);
                for (int f = 0; f < nfound; f++) {
                    int j = foundv[f];
                    adjb[i][j >> 5] |= 1u << (j & 31);
                    adjb[j][i >> 5] |= 1u << (i & 31);
                }
            }
            for (int t = k; t < k2; t++) {
                int i = ord[t];
                tset(1, 0, TOPY, 2 * sy1[i], 2 * sy2[i], i + 1);
            }
            k = k2;
        }

        /* count triples u<v<w that are pairwise visible */
        int total = 0;
        for (int u = 0; u < n; u++) {
            int uw = u >> 5;
            unsigned int umask = ((u & 31) == 31) ? 0u : (0xFFFFFFFFu << ((u & 31) + 1));
            for (int wi = uw; wi < nw; wi++) {
                unsigned int m = adjb[u][wi];
                if (wi == uw) m &= umask;
                while (m) {
                    unsigned int low = m & (~m + 1u);
                    int v = (wi << 5) + debtab[(low * DEB) >> 27];
                    m ^= low;
                    int vw = v >> 5;
                    unsigned int vmask = ((v & 31) == 31) ? 0u : (0xFFFFFFFFu << ((v & 31) + 1));
                    unsigned int x = adjb[u][vw] & adjb[v][vw] & vmask;
                    int cnt = pc16[x & 0xFFFFu] + pc16[x >> 16];
                    for (int t = vw + 1; t < nw; t++) {
                        unsigned int y = adjb[u][t] & adjb[v][t];
                        cnt += pc16[y & 0xFFFFu] + pc16[y >> 16];
                    }
                    total += cnt;
                }
            }
        }
        printf("%d\n", total);
    }
    return 0;
}
