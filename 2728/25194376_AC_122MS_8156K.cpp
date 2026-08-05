// POJ 2728 - Desert King
//
// Optimal ratio spanning tree.  On the complete graph over the N villages an
// edge (i,j) has cost |z_i - z_j| (the lifter height) and length
// dist((x_i,y_i),(x_j,y_j)); we want the spanning tree minimising
// sum(cost)/sum(len).  No fixed edge weighting gives that tree directly, so
// parametrise: the answer is <= lambda iff some spanning tree satisfies
// sum(cost) - lambda*sum(len) <= 0, i.e. iff the MST under the weights
// w(e) = cost(e) - lambda*len(e) has non-positive total weight.  That value
// f(lambda) is strictly decreasing, and its root is the answer.
//
// Solved by Dinkelbach (Newton) rather than bisection: build the MST at the
// current lambda, then set lambda := C/L from that very tree's real cost and
// length.  It converges from above and superlinearly -- 4 iterations on
// random N=1000 inputs -- so an O(N^2) dense Prim per round is plenty inside
// the 3s limit (122MS as submitted).  Pairwise distances are precomputed once
// per test case (8MB of doubles, within the 64MB limit); costs are recomputed
// from the z array since |z_i - z_j| is cheaper than a cache miss.
//
// Statement notes: "average cost per mile" is the ratio of the two sums over
// the whole tree, not the mean of the per-edge ratios.  The input is a stream
// of test cases terminated by N = 0.  The guarantees that altitudes are all
// distinct and that no three villages are collinear are never needed; the
// L == 0 guard below only covers a malformed case where every village shares
// one position.  Output is printf("%.3f") -- three digits, rounded.
#include <cstdio>
#include <cstdlib>
#include <cmath>

static const int MAXN = 1005;

static int X[MAXN], Y[MAXN], Z[MAXN];
static double D[MAXN][MAXN];
static double low[MAXN];
static int pre[MAXN];
static bool used[MAXN];

static int n;

/* Prim on weights  cost(e) - lambda*len(e);  returns actual (C, L) of that tree. */
static void prim(double lambda, double *Cout, double *Lout) {
    int i, k, v, u;
    double best, w, C = 0.0, L = 0.0;

    for (i = 0; i < n; ++i) used[i] = false;
    used[0] = true;
    for (i = 1; i < n; ++i) {
        low[i] = (double)abs(Z[0] - Z[i]) - lambda * D[0][i];
        pre[i] = 0;
    }

    for (k = 1; k < n; ++k) {
        u = -1;
        best = 0.0;
        for (v = 1; v < n; ++v) {
            if (!used[v] && (u == -1 || low[v] < best)) { u = v; best = low[v]; }
        }
        used[u] = true;
        C += (double)abs(Z[pre[u]] - Z[u]);
        L += D[pre[u]][u];
        for (v = 1; v < n; ++v) {
            if (!used[v]) {
                w = (double)abs(Z[u] - Z[v]) - lambda * D[u][v];
                if (w < low[v]) { low[v] = w; pre[v] = u; }
            }
        }
    }
    *Cout = C;
    *Lout = L;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j;
        for (i = 0; i < n; ++i) scanf("%d %d %d", &X[i], &Y[i], &Z[i]);

        for (i = 0; i < n; ++i) {
            D[i][i] = 0.0;
            for (j = i + 1; j < n; ++j) {
                double dx = (double)(X[i] - X[j]);
                double dy = (double)(Y[i] - Y[j]);
                double d = sqrt(dx * dx + dy * dy);
                D[i][j] = d;
                D[j][i] = d;
            }
        }

        double lambda = 0.0, C, L;
        int it;
        for (it = 0; it < 100; ++it) {
            prim(lambda, &C, &L);
            if (L < 1e-12) { lambda = 0.0; break; }
            double nl = C / L;
            if (fabs(nl - lambda) < 1e-10) { lambda = nl; break; }
            lambda = nl;
        }
        printf("%.3f\n", lambda);
    }
    return 0;
}
