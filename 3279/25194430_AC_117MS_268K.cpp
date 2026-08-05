// POJ 3279 - Fliptile
//
// An M x N (both <= 15) grid of black/white tiles; flipping a tile also flips
// its four edge-neighbours. Make every tile white in the fewest flips, and
// among the minimum-flip solutions print the lexicographically least one (the
// output rows read as one string, so that is plain row-major order on the
// 0/1 grid). Print IMPOSSIBLE when no solution exists.
//
// Flips commute and flipping twice is a no-op, so every cell is flipped 0 or 1
// times and only the set matters. Once the first row's flip pattern is fixed,
// the rest is forced: after all flips in rows <= i-1 have been applied, the
// only cell that can still change (i-1, j) is (i, j), so flp[i][j] must be 1
// exactly when (i-1, j) is still black. So enumerate all 2^N first rows
// (<= 32768), propagate downwards in O(M*N), and accept a pattern only if the
// last row comes out all white. Total 2^N * M * N ~ 7.4M steps.
//
// Enumerating the mask with column j taking bit (N-1-j) makes ascending mask
// order equal ascending lexicographic order of the first row, and since
// distinct masks give distinct first rows, a tie in flip count is already
// decided by row 0 -- the explicit row-major comparison below is just belt
// and braces.
//
// Ambiguity: "least lexicographical ordering in the output when considered as
// a string" is only meaningful because the layout is fixed-width (single
// digits, single spaces), which makes string order and row-major order on the
// grid the same thing. Also note the output asks how many *times* to flip each
// cell, but under minimality that count is never more than 1.
#include <cstdio>
#include <cstring>

int M, N;
int orig[16][16];
int flp[16][16];
int best[16][16];
int bestCnt;
int haveBest;

/* state of (r,c) counting only flips in rows <= r */
static int stateOf(int r, int c) {
    int s = orig[r][c] ^ flp[r][c];
    if (r > 0) s ^= flp[r - 1][c];
    if (c > 0) s ^= flp[r][c - 1];
    if (c + 1 < N) s ^= flp[r][c + 1];
    return s;
}

/* lexicographic compare of flp vs best, row-major; <0 means flp is smaller */
static int cmpBest() {
    int i, j;
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            if (flp[i][j] != best[i][j]) return flp[i][j] - best[i][j];
    return 0;
}

int main() {
    if (scanf("%d %d", &M, &N) != 2) return 0;
    int i, j;
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            scanf("%d", &orig[i][j]);

    haveBest = 0;
    bestCnt = 0;
    long total = 1L << N;
    long mask;
    for (mask = 0; mask < total; mask++) {
        memset(flp, 0, sizeof(flp));
        /* column j takes bit (N-1-j) so ascending mask == ascending
           lexicographic order of the first row */
        for (j = 0; j < N; j++)
            flp[0][j] = (int)((mask >> (N - 1 - j)) & 1L);

        int cnt = 0;
        for (j = 0; j < N; j++) cnt += flp[0][j];

        for (i = 1; i < M; i++) {
            for (j = 0; j < N; j++) {
                /* row i-1 is complete once flp[i][j] is decided */
                if (stateOf(i - 1, j)) { flp[i][j] = 1; cnt++; }
                else flp[i][j] = 0;
            }
        }

        int ok = 1;
        for (j = 0; j < N && ok; j++)
            if (stateOf(M - 1, j)) ok = 0;
        if (!ok) continue;

        if (!haveBest || cnt < bestCnt || (cnt == bestCnt && cmpBest() < 0)) {
            haveBest = 1;
            bestCnt = cnt;
            memcpy(best, flp, sizeof(flp));
        }
    }

    if (!haveBest) {
        printf("IMPOSSIBLE\n");
        return 0;
    }
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++)
            printf(j ? " %d" : "%d", best[i][j]);
        printf("\n");
    }
    return 0;
}
