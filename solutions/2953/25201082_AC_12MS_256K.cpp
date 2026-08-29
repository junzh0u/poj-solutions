// POJ 2953 - Nash Equilibrium
// Model: claude-sonnet-5
// Approach: m,n <= 20, so brute force every (i,j) pair. (i,j) is a pure
// strategy Nash equilibrium iff PA[i][j] equals the column max of PA's
// j-th column (a_i is a best response to b_j) and PB[i][j] equals the row
// max of PB's i-th row (b_j is a best response to a_i). Ties count: any
// action achieving the max payoff is a valid best response, matching the
// sample's 2x2 all-ones case which reports all four cells. Enumerate i in
// increasing order then j in increasing order, which is already
// lexicographic, so no separate sort is needed. Read cases until m=n=0.
#include <cstdio>

int PA[21][21], PB[21][21];

int main() {
    int m, n;
    while (scanf("%d %d", &m, &n) == 2 && (m || n)) {
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &PA[i][j]);
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &PB[i][j]);

        int colMaxA[21], rowMaxB[21];
        for (int j = 1; j <= n; j++) {
            int best = PA[1][j];
            for (int i = 2; i <= m; i++)
                if (PA[i][j] > best) best = PA[i][j];
            colMaxA[j] = best;
        }
        for (int i = 1; i <= m; i++) {
            int best = PB[i][1];
            for (int j = 2; j <= n; j++)
                if (PB[i][j] > best) best = PB[i][j];
            rowMaxB[i] = best;
        }

        int resI[401], resJ[401], cnt = 0;
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                if (PA[i][j] == colMaxA[j] && PB[i][j] == rowMaxB[i]) {
                    resI[cnt] = i;
                    resJ[cnt] = j;
                    cnt++;
                }

        printf("%d\n", cnt);
        for (int k = 0; k < cnt; k++)
            printf("%d %d\n", resI[k], resJ[k]);
    }
    return 0;
}
