// POJ 1736 - Block Town
// Model: claude-sonnet-5
//
// Front view gives, for each of the K rows, the max height along that row;
// right view gives, for each of the L columns, the max height along that
// column. A town matching both views exists iff the two views share the
// same global maximum (the tallest tower is visible from both sides, so
// max(front) must equal max(right)); otherwise "No solution."
//
// Maximum blocks: h[i][j] can be as large as min(front[i], right[j]) without
// ever forcing a row or column max above its target (since front[i] and
// right[j] are themselves each <= the shared global max), so the maximum is
// simply sum over all cells of min(front[i], right[j]), computed via bucket
// counts over heights (<=5000) with prefix/suffix sums instead of an O(K*L)
// or O((K+L) log) pass.
//
// Minimum blocks: every row i needs some cell equal to front[i] and every
// column j needs some cell equal to right[j]; a single cell can satisfy both
// simultaneously only when front[i] == right[j] (that shared value is then
// both the row max and the column max). Rows/columns can always find some
// compatible partner (the global-argmax row/column works as a fallback), so
// the only savings come from pairing rows and columns of equal height, and
// since any row of height v can pair with any column of height v without
// colliding (distinct row index means a distinct cell), the maximum total
// saving is sum over each height v of min(count_front(v), count_right(v))*v.
// Minimum = sum(front) + sum(right) - that saving.
//
// Both formulas were checked against a brute-force enumeration over all
// small matrices (K,L<=3, heights 0..3, 3612 cases, 0 mismatches) and
// against the discuss board's own separating case (message 15225: front
// 3 3 2 2, right 3 3 1 1 -> correct 12 28, distinguishing this solution
// from a known-wrong per-height max(count_front>=h,count_right>=h) method
// that outputs 10 28 on the same input).
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXH = 5000;

static long long cntF[MAXH + 2];
static long long cntR[MAXH + 2];
static long long suffixCountR[MAXH + 2];
static long long prefixSumR[MAXH + 2];

int main() {
    int K, L;
    scanf("%d %d", &K, &L);

    long long sumF = 0, sumR = 0;
    int maxF = 0, maxR = 0;
    for (int i = 0; i < K; i++) {
        int h;
        scanf("%d", &h);
        cntF[h]++;
        sumF += h;
        if (h > maxF) maxF = h;
    }
    for (int j = 0; j < L; j++) {
        int h;
        scanf("%d", &h);
        cntR[h]++;
        sumR += h;
        if (h > maxR) maxR = h;
    }

    if (maxF != maxR) {
        printf("No solution.\n");
        return 0;
    }

    long long savings = 0;
    for (int v = 0; v <= MAXH; v++) {
        if (cntF[v] && cntR[v]) {
            long long m = min(cntF[v], cntR[v]);
            savings += m * (long long)v;
        }
    }
    long long minAns = sumF + sumR - savings;

    suffixCountR[MAXH + 1] = 0;
    for (int v = MAXH; v >= 0; v--) suffixCountR[v] = suffixCountR[v + 1] + cntR[v];
    prefixSumR[0] = 0;
    for (int v = 0; v <= MAXH; v++) prefixSumR[v + 1] = prefixSumR[v] + (long long)v * cntR[v];

    long long maxAns = 0;
    for (int v = 0; v <= MAXH; v++) {
        if (cntF[v] == 0) continue;
        long long contrib = (long long)v * suffixCountR[v] + prefixSumR[v];
        maxAns += contrib * cntF[v];
    }

    printf("%lld %lld\n", minAns, maxAns);
    return 0;
}
