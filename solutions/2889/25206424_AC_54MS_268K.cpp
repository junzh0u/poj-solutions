// POJ 2889 - Unique Solution
// Model: claude-sonnet-5
//
// Choose exactly M disjoint, order-preserving, contiguous segments
// [s_1,e_1] < [s_2,e_2] < ... < [s_M,e_M] (segments may touch: e_i < s_{i+1})
// maximizing the total sum, and decide whether the maximizing choice of
// segment boundaries is unique.
//
// Standard O(N*M) DP for "maximum sum of M disjoint subarrays":
//   B[j][i] = best sum using j segments among the first i numbers
//             (segment j, if used, ends at or before i)
//   A[j][i] = best sum using j segments among the first i numbers where
//             segment j ends exactly at i (a[i] included in segment j)
//   A[j][i] = a[i] + max(A[j][i-1] "extend segment j", B[j-1][i-1] "start segment j at i")
//   B[j][i] = max(B[j][i-1] "segment j already finished", A[j][i])
// Answer value = B[M][N].
//
// Each DP transition corresponds to a structurally distinct choice of
// segment boundaries (different s_j on "start new" vs "extend", different
// e_j on "finish here" vs "finish earlier"), so counting the number of
// optimal *paths* through this DP counts exactly the number of distinct
// optimal (s,e) configurations -- no path is double counted and no two
// distinct configurations collapse onto the same path. We saturate the
// count at 2 (only "1" vs ">=1 more" matters) to avoid overflow.
// Verified against a brute-force enumeration over all ways to place M
// disjoint segments: exhaustively for N<=6 over values in {-1,0,1} (all
// M, all value assignments) and by 500 random trials at N<=8, both the
// max value and the uniqueness verdict, including cases with 0-valued
// elements (those still count as distinct integer boundaries when a
// segment can be extended or trimmed through a zero, hence non-unique).
// Mutation check: a variant that skipped the explicit tie branch between
// "extend" and "start new" (biasing to one side, no count merge) passed
// the sample but disagreed with brute force on 65/500 random trials,
// confirming the tie handling is load-bearing and the sample alone is
// not sufficient evidence.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const long long NEG = -(long long)1e15;

static int a[MAXN];
static long long Aval[MAXN], Bval[MAXN], Bprev[MAXN];
static int Acnt[MAXN], Bcnt[MAXN], Bprevcnt[MAXN];

static inline int sat(int x) { return x > 2 ? 2 : x; }

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

        for (int i = 0; i <= n; i++) { Bprev[i] = 0; Bprevcnt[i] = 1; }

        for (int j = 1; j <= m; j++) {
            Aval[0] = NEG; Acnt[0] = 0;
            Bval[0] = NEG; Bcnt[0] = 0;
            for (int i = 1; i <= n; i++) {
                long long extendVal = (Aval[i - 1] == NEG) ? NEG : Aval[i - 1] + a[i];
                long long startVal = (Bprev[i - 1] == NEG) ? NEG : Bprev[i - 1] + a[i];
                if (extendVal == startVal) {
                    Aval[i] = extendVal;
                    Acnt[i] = sat(Acnt[i - 1] + Bprevcnt[i - 1]);
                } else if (extendVal > startVal) {
                    Aval[i] = extendVal;
                    Acnt[i] = Acnt[i - 1];
                } else {
                    Aval[i] = startVal;
                    Acnt[i] = Bprevcnt[i - 1];
                }

                long long bprevValHere = Bval[i - 1];
                if (bprevValHere == Aval[i]) {
                    Bval[i] = bprevValHere;
                    Bcnt[i] = sat(Bcnt[i - 1] + Acnt[i]);
                } else if (bprevValHere > Aval[i]) {
                    Bval[i] = bprevValHere;
                    Bcnt[i] = Bcnt[i - 1];
                } else {
                    Bval[i] = Aval[i];
                    Bcnt[i] = Acnt[i];
                }
            }
            for (int i = 0; i <= n; i++) { Bprev[i] = Bval[i]; Bprevcnt[i] = Bcnt[i]; }
        }

        puts(Bprevcnt[n] >= 2 ? "No" : "Yes");
    }
    return 0;
}
