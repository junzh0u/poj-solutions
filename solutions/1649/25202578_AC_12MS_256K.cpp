// POJ 1649 - Market Place
// Model: claude-sonnet-5
//
// Insert a new post with price P after existing post L (1<=L<N). For a
// walker who inspects the first K posts from one end (K in [1,N], weight
// B_K% of all buyers, split evenly between the two directions), the walker
// buys from the price-minimal post among those K, breaking ties toward the
// post nearer his turning point (largest index for a left-to-right walker,
// smallest index for a right-to-left walker).
//
// For a fixed insertion point L, a left-to-right walker with K<=L never
// reaches the new post. For K>L he sees original posts 1..L, the new post,
// then original L+1..K-1. The new post is chosen iff
//   P <= min(price[1..L])                       (ties broken in its favor:
//                                                  it is nearer the turn)
//   P <  min(price[L+1..K-1])                    (ties broken against it)
// so define threshold_left(L,K) = min(A, B-1) where A=min(price[1..L]),
// B=min(price[L+1..K-1]) (B=+inf, i.e. no upper constraint, when that range
// is empty). threshold_left(L,K) is the maximum P for which K's buyers
// (walking left-to-right) pick the new post; it is non-increasing in K.
//
// The right-to-left case is symmetric on the reversed array with split
// point M=N-L.
//
// Revenue is a piecewise-constant-times-P function of P, so the optimum for
// each L is attained at one of these threshold values; try them all.
// Overall complexity O(N^2), N<=100.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 105;
const int INF = 1000000000;

int N;
int price[MAXN];
int B[MAXN];

// Computes threshold[k] for k=1..N given an array arr[1..N] and split s
// (1<=s<=N-1): threshold[k]=0 for k<=s (new post never reached, so it can
// never be chosen there since prices are positive); for k>s it is as
// described above.
void computeThresholds(int *arr, int s, int *thresh) {
    for (int k = 1; k <= s; k++) thresh[k] = 0;
    int A = INF;
    for (int i = 1; i <= s; i++) A = min(A, arr[i]);
    int runningMin = INF; // acts as +infinity sentinel
    for (int k = s + 1; k <= N; k++) {
        int B_ = runningMin;
        int t;
        if (B_ >= INF) t = A;
        else t = min(A, B_ - 1);
        thresh[k] = (int)t;
        // extend range to include arr[k] for next iteration
        if (arr[k] < runningMin) runningMin = arr[k];
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) scanf("%d", &price[i]);
    for (int i = 1; i <= N; i++) scanf("%d", &B[i]);

    int rev[MAXN];
    for (int i = 1; i <= N; i++) rev[i] = price[N + 1 - i];

    int bestMetric = -1;
    int bestL = -1, bestP = -1;

    int threshL[MAXN], threshR[MAXN];

    for (int L = 1; L <= N - 1; L++) {
        int M = N - L;
        computeThresholds(price, L, threshL);
        computeThresholds(rev, M, threshR);

        // gather candidate positive P values. P=1 is always included: when
        // every K reachable at this L has B_K=0, the whole plateau down to
        // P=1 ties at metric 0, and the tie-break wants the smallest P, not
        // whatever positive threshold happened to be produced.
        int cand[2 * MAXN + 1];
        int nc = 0;
        cand[nc++] = 1;
        for (int k = 1; k <= N; k++) {
            if (threshL[k] > 0) cand[nc++] = threshL[k];
            if (threshR[k] > 0) cand[nc++] = threshR[k];
        }
        sort(cand, cand + nc);
        nc = (int)(unique(cand, cand + nc) - cand);

        for (int ci = 0; ci < nc; ci++) {
            int P = cand[ci];
            int SL = 0, SR = 0;
            for (int k = 1; k <= N; k++) {
                if (threshL[k] >= P) SL += B[k];
                if (threshR[k] >= P) SR += B[k];
            }
            int metric = P * (SL + SR);
            if (metric > bestMetric ||
                (metric == bestMetric && (L < bestL || (L == bestL && P < bestP)))) {
                bestMetric = metric;
                bestL = L;
                bestP = P;
            }
        }
    }

    if (bestL < 0) {
        // degenerate: no candidate ever earns anything; fall back
        bestL = 1;
        bestP = 1;
    }

    printf("%d %d\n", bestL, bestP);
    return 0;
}
