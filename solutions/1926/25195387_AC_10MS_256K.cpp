// POJ 1926 - Pollution
// Model: claude-opus-5
//
// The update rule X_i^{t+1} = sum_j I_ij X_j^t / d_j is X <- A D^{-1} X, i.e. a
// random walk on the graph run on the mass vector.  Two facts settle it in
// closed form, with no iteration at all:
//
//   * Mass is conserved inside each connected component:
//     sum_i X_i^{t+1} = sum_j (X_j / d_j) * sum_i I_ij = sum_j X_j.
//   * The degree vector is a fixed point: (A D^{-1} d)_i = sum_j I_ij = d_i.
//
// On a connected component the eigenvalue-1 eigenspace of A D^{-1} is
// one-dimensional, so the equilibrium is forced: every tank i in a component
// with total mass S and degree sum D ends at X_i = S * d_i / D.  An isolated
// tank (d_i = 0) is its own component and the statement fixes it explicitly:
// X_i^{t+1} = X_i^t, so it keeps its initial amount.  Forgetting that case is
// the trap the discuss board warns about most.
//
// Bipartite components never actually converge -- a path 1-2-3 starting at
// (1,0,0) oscillates forever -- but the statement says to assume equilibrium is
// reached, and the Cesaro limit (average of two consecutive iterates) is
// exactly the fixed point above, so the same formula answers those too.
//
// The initial amounts are REAL numbers, not integers (another board trap), read
// with scanf("%lf").  Output is one line per tank plus a blank line after every
// test case, including the last.
//
// Rounding note: values like S*d/D can land exactly on a 3-decimal tie
// (e.g. 171.594 * 2 / 8 = 42.8985).  Half-up on the exact decimal and plain
// printf on the double disagree there, so this keeps printf's rounding of the
// double, computed in the same order (component sum, times degree, divided by
// degree sum) as the known-Accepted solutions.  Uses %.3f rather than %.3lf,
// which is unreliable on this judge's compilers.
//
// O(N^2) per test case; trivially inside the limits.

#include <cstdio>

const int MAXN = 105;

bool adj[MAXN][MAXN];
double x[MAXN];
int deg[MAXN];
int par[MAXN];

int findp(int a) {
    while (par[a] != a) {
        par[a] = par[par[a]];
        a = par[a];
    }
    return a;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n, m;
        scanf("%d %d", &n, &m);
        for (int i = 1; i <= n; i++) {
            scanf("%lf", &x[i]);
            par[i] = i;
            deg[i] = 0;
            for (int j = 1; j <= n; j++) adj[i][j] = false;
        }
        for (int e = 0; e < m; e++) {
            int a, b;
            scanf("%d %d", &a, &b);
            if (a == b) continue;
            if (adj[a][b]) continue;   /* statement promises at most one pipe; dedupe anyway */
            adj[a][b] = adj[b][a] = true;
            deg[a]++;
            deg[b]++;
            int ra = findp(a), rb = findp(b);
            if (ra != rb) par[ra] = rb;
        }
        double sumX[MAXN];
        double sumD[MAXN];
        for (int i = 1; i <= n; i++) { sumX[i] = 0.0; sumD[i] = 0.0; }
        for (int i = 1; i <= n; i++) {
            int r = findp(i);
            sumX[r] += x[i];
            sumD[r] += (double)deg[i];
        }
        for (int i = 1; i <= n; i++) {
            int r = findp(i);
            double v;
            if (deg[i] == 0) v = x[i];               /* isolated tank keeps its own amount */
            else v = sumX[r] * (double)deg[i] / sumD[r];
            printf("%.3f\n", v);
        }
        printf("\n");
    }
    return 0;
}
