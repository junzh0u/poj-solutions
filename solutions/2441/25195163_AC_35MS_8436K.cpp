// POJ 2441 - Arrange the Bulls
// Model: claude-opus-5
//
// Count perfect matchings of cows into distinct barns (N, M <= 20).
//
// Approach: bitmask DP over the set of *occupied barns*. Because cows are
// assigned in index order, popcount(mask) alone tells us how many cows are
// already placed, so the cow index needs no dimension of its own: one array
// dp[1<<M] suffices instead of dp[N][1<<M] (the usual TLE/MLE trap here --
// the discuss board is full of O(N*M*2^M) attempts timing out).
//
//   dp[mask] = number of ways to place cows 0..popcount(mask)-1 into exactly
//              the barns of mask.
//   dp[0] = 1; from each mask with k = popcount(mask) < N, push cow k into
//   every barn it likes that mask does not already use.
//   Answer = sum of dp[mask] over popcount(mask) == N.
//
// One sweep of the masks in increasing order visits each state after all its
// predecessors, so a single pass suffices: O(2^M * M) ~ 2*10^7, ~0.01s locally.
//
// Notes:
//  - Counts are accumulated in long long. The statement caps the *answer* at
//    10^7, but partial counts are bounded only by P(20,20) = 20! ~ 2.4*10^18,
//    which still fits. The printed answer is cast to int per the cap.
//  - A cow's list is folded into a bitmask, so a repeated barn number counts
//    once ("P referring to the number of barns cow i likes").
//  - N > M and unmatchable inputs fall out as 0 with no special case.
//  - Input is read to EOF in case the judge data holds several data sets.
//  - Submitted under language 0 (G++): the board reports C++ TLE / G++ AC,
//    and POJ's "C++" front end has no long long.

#include <cstdio>
#include <cstring>

static long long dp[1 << 20];
static int like[20];

static int pc(unsigned int x) {
    x = x - ((x >> 1) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
    x = (x + (x >> 4)) & 0x0f0f0f0fu;
    return (int)((x * 0x01010101u) >> 24);
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int i;
        for (i = 0; i < n; ++i) {
            int p, j;
            like[i] = 0;
            scanf("%d", &p);
            for (j = 0; j < p; ++j) {
                int b;
                scanf("%d", &b);
                if (b >= 1 && b <= m) like[i] |= 1 << (b - 1);
            }
        }
        int full = 1 << m;
        memset(dp, 0, sizeof(long long) * (size_t)full);
        dp[0] = 1;
        long long ans = 0;
        int mask;
        for (mask = 0; mask < full; ++mask) {
            long long cur = dp[mask];
            if (cur == 0) continue;
            int k = pc((unsigned int)mask);
            if (k == n) { ans += cur; continue; }
            if (k > n) continue;
            int avail = like[k] & ~mask;
            while (avail) {
                int b = avail & (-avail);
                dp[mask | b] += cur;
                avail ^= b;
            }
        }
        printf("%d\n", (int)ans);
    }
    return 0;
}
