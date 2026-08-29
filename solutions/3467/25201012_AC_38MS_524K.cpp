// POJ 3467 - Cross Counting
// Model: claude-sonnet-5
// For each cell, a cross of size k>=1 centered there requires the k nearest
// cells in its row and column (both directions) to match its own color.
// The maximum feasible k at a cell equals min(L,R,U,D), the run-length of
// same-color cells extending left/right/up/down (bounded by grid edges),
// and every k from 1..max contributes a distinct cross (k=0 never counts,
// confirmed against the sample: corner cells with differing neighbors give 0).
// Changing one cell's color only changes L/R for its own row and U/D for its
// own column (segments elsewhere are untouched), so an update only needs to
// recompute that row and column (O(N+M)) and fix up the per-color sums for
// exactly the cells in that row/column, rather than rescanning the grid.
#include <cstdio>
#include <cstring>

const int MAXN = 105;
int N, M, C, Q;
int color_[MAXN][MAXN];
int L[MAXN][MAXN], R[MAXN][MAXN], U[MAXN][MAXN], D[MAXN][MAXN];
int kmax_[MAXN][MAXN];
int sum_[105];

void recomputeRow(int i) {
    for (int j = 1; j <= M; j++) {
        if (j == 1 || color_[i][j-1] != color_[i][j]) L[i][j] = 0;
        else L[i][j] = L[i][j-1] + 1;
    }
    for (int j = M; j >= 1; j--) {
        if (j == M || color_[i][j+1] != color_[i][j]) R[i][j] = 0;
        else R[i][j] = R[i][j+1] + 1;
    }
}

void recomputeCol(int j) {
    for (int i = 1; i <= N; i++) {
        if (i == 1 || color_[i-1][j] != color_[i][j]) U[i][j] = 0;
        else U[i][j] = U[i-1][j] + 1;
    }
    for (int i = N; i >= 1; i--) {
        if (i == N || color_[i+1][j] != color_[i][j]) D[i][j] = 0;
        else D[i][j] = D[i+1][j] + 1;
    }
}

int computeK(int i, int j) {
    int m = L[i][j];
    if (R[i][j] < m) m = R[i][j];
    if (U[i][j] < m) m = U[i][j];
    if (D[i][j] < m) m = D[i][j];
    return m;
}

int main(){
    scanf("%d %d %d %d", &N, &M, &C, &Q);
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
            scanf("%d", &color_[i][j]);

    for (int i = 1; i <= N; i++) recomputeRow(i);
    for (int j = 1; j <= M; j++) recomputeCol(j);

    memset(sum_, 0, sizeof(sum_));
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++) {
            kmax_[i][j] = computeK(i,j);
            sum_[color_[i][j]] += kmax_[i][j];
        }

    for (int q = 0; q < Q; q++) {
        char cmd[8];
        scanf("%s", cmd);
        if (cmd[0] == 'Q') {
            int c;
            scanf("%d", &c);
            printf("%d\n", sum_[c]);
        } else {
            int x, y, k;
            scanf("%d %d %d", &x, &y, &k);
            if (color_[x][y] == k) continue;
            for (int j = 1; j <= M; j++) {
                sum_[color_[x][j]] -= kmax_[x][j];
            }
            for (int i = 1; i <= N; i++) {
                if (i == x) continue;
                sum_[color_[i][y]] -= kmax_[i][y];
            }
            color_[x][y] = k;
            recomputeRow(x);
            recomputeCol(y);
            for (int j = 1; j <= M; j++) {
                kmax_[x][j] = computeK(x,j);
                sum_[color_[x][j]] += kmax_[x][j];
            }
            for (int i = 1; i <= N; i++) {
                if (i == x) continue;
                kmax_[i][y] = computeK(i,y);
                sum_[color_[i][y]] += kmax_[i][y];
            }
        }
    }
    return 0;
}
