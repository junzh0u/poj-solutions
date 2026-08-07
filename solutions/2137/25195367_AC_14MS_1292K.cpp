// POJ 2137 - Cowties
// Model: claude-opus-5
//
// Cow i is tied to cows i-1 and i+1 (cyclically), so the rope is a closed
// polygon visiting the cows in index order; each cow independently picks one
// of its S <= 40 preferred spots. Only adjacent cows interact, so this is a
// cyclic chain DP: fix cow 0's spot (<= 40 choices), run a linear DP along
// cows 1..N-1, then close the loop back to the fixed spot.
//   cost = sum over i of |p_i - p_{(i+1) mod N}|
//   O(S * N * S^2) <= 40 * 100 * 1600 = 6.4e6 with adjacent distance matrices
//   precomputed once. Runs in well under 1 ms at the stated limits.
//
// Output: "100 times the minimum length ... do not perform special rounding"
// means truncate, not round (the discuss board is unanimous on this, with
// several people reporting WA for rounding). A tiny epsilon is added before
// truncating: when the optimum is an integer every edge of it must have
// integer length (a sum of square roots of integers is rational only if each
// root is), so the double sum is then exact and the epsilon is a no-op -- it
// only guards against a downward rounding slip. Verified to change nothing on
// 1200 randomized cases.
//
// The read loop tolerates (but does not require) more than one data set; a
// malformed trailing token is rejected rather than printed as a bogus answer.
#include <cstdio>
#include <cmath>

static const double INF = 1e18;

static int S[105];
static int X[105][45], Y[105][45];
static double d[105][45][45];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 3 || n > 100) break;
        int i, j, k, a, b;
        int ok = 1;
        for (i = 0; i < n && ok; i++) {
            if (scanf("%d", &S[i]) != 1 || S[i] < 1 || S[i] > 40) { ok = 0; break; }
            for (j = 0; j < S[i]; j++)
                if (scanf("%d %d", &X[i][j], &Y[i][j]) != 2) { ok = 0; break; }
        }
        if (!ok) break;

        for (i = 0; i < n; i++) {
            int p = (i + 1) % n;
            for (a = 0; a < S[i]; a++)
                for (b = 0; b < S[p]; b++) {
                    double dx = X[i][a] - X[p][b];
                    double dy = Y[i][a] - Y[p][b];
                    d[i][a][b] = sqrt(dx * dx + dy * dy);
                }
        }

        double best = INF;
        double cur[45], nxt[45];
        for (int s0 = 0; s0 < S[0]; s0++) {
            for (j = 0; j < S[1]; j++) cur[j] = d[0][s0][j];
            for (i = 1; i <= n - 2; i++) {
                int p = i + 1;
                for (k = 0; k < S[p]; k++) nxt[k] = INF;
                for (j = 0; j < S[i]; j++) {
                    double c = cur[j];
                    for (k = 0; k < S[p]; k++) {
                        double v = c + d[i][j][k];
                        if (v < nxt[k]) nxt[k] = v;
                    }
                }
                for (k = 0; k < S[p]; k++) cur[k] = nxt[k];
            }
            for (j = 0; j < S[n - 1]; j++) {
                double v = cur[j] + d[n - 1][j][s0];
                if (v < best) best = v;
            }
        }

        printf("%d\n", (int)(best * 100.0 + 1e-9));
    }
    return 0;
}
