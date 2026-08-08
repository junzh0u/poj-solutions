// POJ 2430 - Lazy Cows
// Model: claude-opus-5
//
// 2 x B grid, N cows, at most K pairwise-disjoint rectangular barns covering
// every cow, minimise total area.  ("just K" and "at most K" coincide: total
// area is at least N >= K, and any barn of area >= 2 splits into two barns of
// the same total area, so a cheaper solution using fewer barns can always be
// padded up to exactly K.)
//
// In a 2-row grid a barn is either height 1 (one row) or height 2, and
// disjointness means a column is covered by at most one barn per row.  So the
// set of barns crossing a given column is fully described by five states, and
// the barns are modelled directly rather than through any covered-region
// argument.  Every barn edge sits on a cow column (otherwise shrink the barn),
// so the sweep only needs the distinct cow columns; between two of them a barn
// either runs through, paying its rows times the gap width, or stops and a new
// barn opens.  dp[barns used][state] over the compressed columns, rolling.
//
// O(M*K*25) time with M <= N distinct columns, O(K) memory.  Robustness that
// costs nothing: the cases are read to EOF, the row index is taken as
// "1 or not 1" (the problem is symmetric under swapping the two rows), and no
// array is sized from the stated bounds.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

/* States: which barns cover the current column.
   0 = nothing
   1 = a height-1 barn in row 0 only
   2 = a height-1 barn in row 1 only
   3 = two height-1 barns, one per row
   4 = a single height-2 barn                            */

static const int coverMask[5] = {0, 1, 2, 3, 3};
static const int rowsCov[5]   = {0, 1, 1, 2, 2};
/* per state: does it hold a row0-only barn, a row1-only barn, a height-2 barn */
static const int hasA[5] = {0, 1, 0, 1, 0};
static const int hasB[5] = {0, 0, 1, 1, 0};
static const int hasC[5] = {0, 0, 0, 0, 1};

static const int INF = 1000000000;

/* rows are 0/1, cols are 1..B; returns the minimum total barn area */
static int solveDP(int n, int k, const int *row, const int *col) {
    std::vector<int> cs(col, col + n);
    std::sort(cs.begin(), cs.end());
    cs.erase(std::unique(cs.begin(), cs.end()), cs.end());
    int m = (int)cs.size();
    std::vector<int> need(m, 0);
    for (int i = 0; i < n; ++i) {
        int p = (int)(std::lower_bound(cs.begin(), cs.end(), col[i]) - cs.begin());
        need[p] |= (1 << row[i]);
    }

    /* For every (s, s') the possible (gapRows, newBarns) trade-offs, with the
       dominated ones dropped.  A barn present in both states may either run on
       through the gap (paying gap cells, no new barn) or stop and restart. */
    int trCnt[5][5];
    int trGap[5][5][4], trNew[5][5][4];
    for (int s = 0; s < 5; ++s)
        for (int t = 0; t < 5; ++t) {
            int best[4];
            for (int i = 0; i < 4; ++i) best[i] = 100;
            int shA = hasA[s] && hasA[t], shB = hasB[s] && hasB[t], shC = hasC[s] && hasC[t];
            for (int ca = 0; ca <= (shA ? 1 : 0); ++ca)
                for (int cb = 0; cb <= (shB ? 1 : 0); ++cb)
                    for (int cc = 0; cc <= (shC ? 1 : 0); ++cc) {
                        int gap = ca + cb + 2 * cc;
                        int nb = (hasA[t] - ca) + (hasB[t] - cb) + (hasC[t] - cc);
                        if (gap < best[nb]) best[nb] = gap;
                    }
            int cnt = 0, minGapSmaller = 100;
            for (int nb = 0; nb <= 3; ++nb) {
                if (best[nb] == 100) continue;
                if (best[nb] < minGapSmaller) {
                    minGapSmaller = best[nb];
                    trGap[s][t][cnt] = best[nb];
                    trNew[s][t][cnt] = nb;
                    ++cnt;
                }
            }
            trCnt[s][t] = cnt;
        }

    int kk = k < 2 * m ? k : 2 * m;
    std::vector<int> cur((kk + 1) * 5, INF), nxt((kk + 1) * 5, INF);

    for (int s = 0; s < 5; ++s) {
        if ((coverMask[s] & need[0]) != need[0]) continue;
        int nb = hasA[s] + hasB[s] + hasC[s];
        if (nb > kk) continue;
        if (rowsCov[s] < cur[nb * 5 + s]) cur[nb * 5 + s] = rowsCov[s];
    }

    for (int i = 1; i < m; ++i) {
        int g = cs[i] - cs[i - 1] - 1;
        std::fill(nxt.begin(), nxt.end(), INF);
        int kcap = kk < 2 * i ? kk : 2 * i;
        for (int kb = 0; kb <= kcap; ++kb) {
            const int base = kb * 5;
            for (int s = 0; s < 5; ++s) {
                int v = cur[base + s];
                if (v >= INF) continue;
                for (int t = 0; t < 5; ++t) {
                    if ((coverMask[t] & need[i]) != need[i]) continue;
                    int cnt = trCnt[s][t];
                    for (int p = 0; p < cnt; ++p) {
                        int nk = kb + trNew[s][t][p];
                        if (nk > kk) continue;
                        int cost = v + g * trGap[s][t][p] + rowsCov[t];
                        if (cost < nxt[nk * 5 + t]) nxt[nk * 5 + t] = cost;
                    }
                }
            }
        }
        cur.swap(nxt);
    }

    int ans = INF;
    for (int kb = 0; kb <= kk; ++kb)
        for (int s = 0; s < 5; ++s)
            if (cur[kb * 5 + s] < ans) ans = cur[kb * 5 + s];
    return ans;
}

int main() {
    int n, k, b;
    while (scanf("%d %d %d", &n, &k, &b) == 3 && n > 0) {
        (void)b;
        std::vector<int> row, col;
        for (int i = 0; i < n; ++i) {
            int r, c;
            if (scanf("%d %d", &r, &c) != 2) break;
            row.push_back(r == 1 ? 0 : 1);
            col.push_back(c);
        }
        int cnt = (int)row.size();
        if (cnt == 0) { printf("0\n"); continue; }
        printf("%d\n", solveDP(cnt, k, &row[0], &col[0]));
    }
    return 0;
}
