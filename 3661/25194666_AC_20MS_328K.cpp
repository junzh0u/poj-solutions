// POJ 3661 - Running
// Model: claude-sonnet-5
// Approach: "She cannot commence running again until her exhaustion factor
// reaches 0" means once resting begins she is locked into resting every
// minute until exhaustion hits 0 (no free interleaving of run/rest). So an
// optimal schedule decomposes into: idle minutes at exhaustion 0, and
// "blocks" of a run of length r (1<=r<=M) immediately followed by exactly r
// minutes of rest that bring exhaustion back to 0. dp[i] = best distance
// using the first i minutes with exhaustion 0 at time i:
//   dp[i] = max( dp[i-1],                                  // idle minute
//                max_{r=1..min(M,i/2)} dp[i-2r] + sum(D[i-2r+1..i-r]) )
// using prefix sums for the block's running-distance sum. Answer = dp[N].
// First submission (25194664) used a free-interleaving DP and got Wrong
// Answer on this problem; verified against sample (dp[5]=9 by hand) and a
// brute force that models the forced-rest-to-zero rule before resubmitting.
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    static int prefix[10001];
    prefix[0] = 0;
    for (int i = 1; i <= n; ++i) {
        int di;
        scanf("%d", &di);
        prefix[i] = prefix[i - 1] + di;
    }

    static int dp[10001];
    dp[0] = 0;
    for (int i = 1; i <= n; ++i) {
        int best = dp[i - 1];
        int maxR = min(m, i / 2);
        for (int r = 1; r <= maxR; ++r) {
            int cand = dp[i - 2 * r] + (prefix[i - r] - prefix[i - 2 * r]);
            if (cand > best) best = cand;
        }
        dp[i] = best;
    }

    printf("%d\n", dp[n]);
    return 0;
}
