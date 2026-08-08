// POJ 3133 - Manhattan Wiring
// Model: claude-opus-5
//
// Broken-profile (plug) DP over the grid in row-major order.  The contour
// carries m+1 plugs, each stored in 2 bits: 0 = no wire, 1 = the "2" wire,
// 2 = the "3" wire.  At each cell the two incoming plugs (left, up) and the
// two outgoing ones (down, right) are chosen so that
//   - an obstacle carries no wire,
//   - a cell marked 2 or 3 has degree exactly 1 in its own colour,
//   - any other used cell has degree exactly 2, all four incident plugs
//     sharing one colour.
// Cost is one per plug created, i.e. one per cell border crossed.  The answer
// is the cheapest way to reach the empty contour after the last cell.
//
// No connectivity information is tracked, and none is needed.  Under the rules
// above the edges of one colour form a graph whose only odd-degree vertices are
// that colour's two marked cells, so it is one path joining them plus disjoint
// cycles; deleting a cycle keeps the configuration legal and strictly shortens
// it, so minimising over this relaxation gives the true optimum.  A cell only
// ever carries one colour, so the two wires are automatically cell-disjoint.
//
// Reading fixed by the samples: a wire may not pass through a marked cell -
// not the other colour's, and not its own.  Dataset 4 of the sample separates
// this from the looser rule (12 versus 10); an exhaustive check over all
// 3x4 boards does not.
//
// Verified against a from-the-statement brute force (enumerate every simple
// path for the "2" wire, BFS the "3" wire over what is left): exhaustively
// over every 3x3, 2x4, 4x2, 2x5, 2x6 and 3x4 board (~1.6M cases) and on
// randomised 4x4..6x6 boards.

#include <cstdio>
#include <vector>
using namespace std;

const int MAXS = 1 << 20; /* 4^(9+1) */

int n, m;
int g[10][10];

static int dpA[MAXS], dpB[MAXS];
static int stA[MAXS], stB[MAXS];
static int *dc, *dn, *sc, *sn;
static int stamp;
static vector<int> listc, listn;

static void relax(int st, int cost) {
    if (sn[st] != stamp) {
        sn[st] = stamp;
        dn[st] = cost;
        listn.push_back(st);
    } else if (cost < dn[st]) {
        dn[st] = cost;
    }
}

int solve() {
    dc = dpA; dn = dpB; sc = stA; sn = stB;
    ++stamp;
    listc.clear();
    sc[0] = stamp; dc[0] = 0; listc.push_back(0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ++stamp;
            listn.clear();
            int shL = 2 * j, shU = 2 * (j + 1);
            int clr = ~((3 << shL) | (3 << shU));
            int cell = g[i][j];
            bool lastRow = (i == n - 1), lastCol = (j == m - 1);
            for (size_t t = 0; t < listc.size(); ++t) {
                int st = listc[t];
                int c = dc[st];
                int L = (st >> shL) & 3;
                int U = (st >> shU) & 3;
                int base = st & clr;
                if (cell == 1) {
                    if (L == 0 && U == 0) relax(base, c);
                } else if (cell == 0) {
                    if (L == 0 && U == 0) {
                        relax(base, c);                    /* unused */
                        if (!lastRow && !lastCol) {        /* corner down+right */
                            for (int col = 1; col <= 2; ++col)
                                relax(base | (col << shL) | (col << shU), c + 2);
                        }
                    } else if (L != 0 && U != 0) {         /* corner left+up */
                        if (L == U) relax(base, c);
                    } else {                               /* one in, one out */
                        int col = L ? L : U;
                        if (!lastRow) relax(base | (col << shL), c + 1);
                        if (!lastCol) relax(base | (col << shU), c + 1);
                    }
                } else {                                   /* marked cell */
                    int col = cell - 1;
                    if (L == 0 && U == 0) {
                        if (!lastRow) relax(base | (col << shL), c + 1);
                        if (!lastCol) relax(base | (col << shU), c + 1);
                    } else if (L != 0 && U != 0) {
                        /* degree 2: a marked cell is an endpoint, so reject */
                    } else {
                        int p = L ? L : U;
                        if (p == col) relax(base, c);
                    }
                }
            }
            int *td = dc; dc = dn; dn = td;
            int *ts = sc; sc = sn; sn = ts;
            listc.swap(listn);
        }
        if (i + 1 < n) {                                   /* shift the contour */
            ++stamp;
            listn.clear();
            int shTop = 2 * m;
            for (size_t t = 0; t < listc.size(); ++t) {
                int st = listc[t];
                if (((st >> shTop) & 3) != 0) continue;
                relax(st << 2, dc[st]);
            }
            int *td = dc; dc = dn; dn = td;
            int *ts = sc; sc = sn; sn = ts;
            listc.swap(listn);
        }
    }
    if (sc[0] != stamp) return 0;                          /* no legal wiring */
    return dc[0];
}

int main() {
    while (scanf("%d %d", &n, &m) == 2 && (n || m)) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                scanf("%d", &g[i][j]);
        printf("%d\n", solve());
    }
    return 0;
}
