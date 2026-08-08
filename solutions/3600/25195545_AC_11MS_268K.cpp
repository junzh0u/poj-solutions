// POJ 3600 - Subimage Recognition
// Model: claude-opus-5
//
// A is a subimage of B iff there are increasing row indices i_1<..<i_r and
// increasing column indices j_1<..<j_c with B[i_a][j_b] == A[a][b].  The two
// axes interact, so no pure greedy works; but fixing one axis makes the other
// a plain 1-D subsequence match, which greedy solves exactly.  So: enumerate
// the row subsets by DFS (C(20,10) = 184756 leaves at worst), maintaining for
// each column of B the bit vector of the rows chosen so far, and at each node
// greedily test whether A's column vectors appear in order among B's.
//
// That test is exact at every prefix depth k, and feasibility is monotone
// decreasing in k (a full witness restricted to its first k rows is a witness
// at depth k), so a failure prunes the whole subtree.  Even with no pruning at
// all the node count is sum_k C(R,k) <= 2^20 at O(C) work each, so the running
// time is bounded by ~4e7 operations regardless of the data.  The axis with
// the smaller binomial is the one enumerated; both matrices are transposed
// when that is the column axis.
//
// No statement ambiguity: rows and columns are deleted independently and the
// surviving ones keep their relative order.

#include <cstdio>
#include <cstring>

static int r, c, R, C;
static char Am[25][25], Bm[25][25];
static int acol[25];      /* acol[s]: bit j = A[j][s] */
static int vec[25][25];   /* vec[k][t]: bit j = B[chosen_j][t], j < k */
static bool found;

static bool feas(int k)
{
    int mask = (1 << k) - 1;
    int s = 0;
    int t;
    for (t = 0; t < C && s < c; t++)
        if (vec[k][t] == (acol[s] & mask)) s++;
    return s == c;
}

static void dfs(int pos, int k)
{
    int i, t;
    if (!feas(k)) return;
    if (k == r) { found = true; return; }
    for (i = pos; i + (r - k) <= R; i++) {
        int bit = 1 << k;
        for (t = 0; t < C; t++)
            vec[k + 1][t] = vec[k][t] | (Bm[i][t] == '1' ? bit : 0);
        dfs(i + 1, k + 1);
        if (found) return;
    }
}

static double binom(int n, int k)
{
    double v = 1.0;
    int i;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) v = v * (n - i) / (i + 1);
    return v;
}

int main()
{
    int i, j;
    char buf[64];
    scanf("%d %d", &r, &c);
    for (i = 0; i < r; i++) { scanf("%s", buf); memcpy(Am[i], buf, c); }
    scanf("%d %d", &R, &C);
    for (i = 0; i < R; i++) { scanf("%s", buf); memcpy(Bm[i], buf, C); }

    /* Enumerate along whichever axis has fewer subsets; transpose if needed. */
    if (binom(C, c) < binom(R, r)) {
        static char TA[25][25], TB[25][25];
        for (i = 0; i < r; i++)
            for (j = 0; j < c; j++) TA[j][i] = Am[i][j];
        for (i = 0; i < R; i++)
            for (j = 0; j < C; j++) TB[j][i] = Bm[i][j];
        memcpy(Am, TA, sizeof(Am));
        memcpy(Bm, TB, sizeof(Bm));
        i = r; r = c; c = i;
        i = R; R = C; C = i;
    }

    for (j = 0; j < c; j++) {
        int m = 0;
        for (i = 0; i < r; i++)
            if (Am[i][j] == '1') m |= 1 << i;
        acol[j] = m;
    }

    found = false;
    dfs(0, 0);
    printf("%s\n", found ? "Yes" : "No");
    return 0;
}
