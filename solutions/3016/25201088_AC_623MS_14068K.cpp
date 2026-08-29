// POJ 3016 - K-Monotonic
// Model: claude-sonnet-5
//
// Split the sequence into k contiguous strictly-monotonic pieces, minimizing
// the total number of +-1 operations. For each candidate segment we need the
// minimum L1 cost to force it to be either strictly increasing or strictly
// decreasing.
//
// Strict monotonicity is turned into weak (non-strict) monotonicity with the
// classic index-shift trick: for strictly increasing, B_i = A_i - i must
// become non-decreasing; for strictly decreasing, C_i = A_i + i must become
// non-increasing (equivalently D_i = -C_i non-decreasing). The shift is by a
// function of the *global* index, but since it only matters up to an additive
// constant on a contiguous range, the same precomputed B/D arrays work for
// every candidate segment start.
//
// The minimum L1 cost to make a sequence non-decreasing is computed with the
// standard single max-heap greedy (as in the "minimum changes to make an
// array non-decreasing, minimizing sum of |delta|" problem): push each new
// value; if it is less than the heap's current max, pay the difference and
// replace the max with the new value. Running this once per candidate start
// index l (over the suffix starting at l+1) fills a cost[l][i] table in
// O(n^2 log n), which is fast enough for n <= 1000.
//
// Finally dp[i][j] = min over l of dp[l][j-1] + min(costInc[l][i], costDec[l][i]),
// an O(n^2 k) DP. Segments have length >= 1 (l < i).
#include <cstdio>
#include <algorithm>
using namespace std;

static const int MAXN = 1005;
static const int MAXK = 11;
static long long costInc[MAXN][MAXN];
static long long costDec[MAXN][MAXN];
static long long dp[MAXN][MAXK];
static long long A[MAXN], B[MAXN], D[MAXN];

// Manual array-backed max-heap (avoids std::priority_queue's repeated
// reallocation overhead when a fresh heap is built for every start index).
static long long heapArr[MAXN];
static int heapSize;

static inline void heapPush(long long x) {
    int i = heapSize++;
    heapArr[i] = x;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (heapArr[p] < heapArr[i]) {
            swap(heapArr[p], heapArr[i]);
            i = p;
        } else break;
    }
}

static inline void heapReplaceTop(long long x) {
    heapArr[0] = x;
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = 2 * i + 2, m = i;
        if (l < heapSize && heapArr[l] > heapArr[m]) m = l;
        if (r < heapSize && heapArr[r] > heapArr[m]) m = r;
        if (m == i) break;
        swap(heapArr[i], heapArr[m]);
        i = m;
    }
}

int main() {
    int n, k;
    while (scanf("%d %d", &n, &k) == 2) {
        if (n == 0 && k == 0) break;
        for (int i = 1; i <= n; i++) scanf("%lld", &A[i]);
        for (int i = 1; i <= n; i++) {
            B[i] = A[i] - i;
            D[i] = -(A[i] + i);
        }
        for (int l = 0; l < n; l++) {
            heapSize = 0;
            long long cost = 0;
            for (int i = l + 1; i <= n; i++) {
                long long x = B[i];
                heapPush(x);
                if (heapArr[0] > x) {
                    cost += heapArr[0] - x;
                    heapReplaceTop(x);
                }
                costInc[l][i] = cost;
            }
        }
        for (int l = 0; l < n; l++) {
            heapSize = 0;
            long long cost = 0;
            for (int i = l + 1; i <= n; i++) {
                long long x = D[i];
                heapPush(x);
                if (heapArr[0] > x) {
                    cost += heapArr[0] - x;
                    heapReplaceTop(x);
                }
                costDec[l][i] = cost;
            }
        }
        const long long INF = (long long)4e18;
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= k; j++) dp[i][j] = INF;
        dp[0][0] = 0;
        for (int j = 1; j <= k; j++) {
            for (int i = j; i <= n; i++) {
                long long best = INF;
                for (int l = j - 1; l < i; l++) {
                    if (dp[l][j - 1] >= INF) continue;
                    long long c = min(costInc[l][i], costDec[l][i]);
                    long long val = dp[l][j - 1] + c;
                    if (val < best) best = val;
                }
                dp[i][j] = best;
            }
        }
        printf("%lld\n", dp[n][k]);
    }
    return 0;
}
