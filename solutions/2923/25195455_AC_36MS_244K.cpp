// POJ 2923 - Relocation
// Model: claude-sonnet-5
// Approach: n<=10 pieces of furniture, two cars with capacities C1,C2.
// Each item is permanently assigned to one of the two cars for the whole
// move (an item that only fits one car has no choice). For a fixed
// assignment, the number of trips a car needs is the classic bin-packing
// "minimum number of bins" for its assigned subset, computed once per
// capacity via subset DP: dp[mask] = 1 + min over nonempty submasks s of
// mask with sum(s) <= C of dp[mask ^ s] (submask enumeration, O(3^n)).
// The answer for a fixed partition is max(dp1[maskToCar1], dp2[rest]);
// we try all 2^n partitions and take the minimum. Masks containing an
// item too heavy for that car get dp = INF and are naturally excluded
// from the min, so no separate feasibility check is needed; the problem
// guarantees every item fits at least one car, so a finite answer exists.
#include <cstdio>
#include <cstring>
using namespace std;

const int INF = 1000000000;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; tc++) {
        int n, C1, C2;
        scanf("%d %d %d", &n, &C1, &C2);
        int w[10];
        for (int i = 0; i < n; i++) scanf("%d", &w[i]);
        int full = (1 << n) - 1;
        int sum[1024];
        sum[0] = 0;
        for (int mask = 1; mask <= full; mask++) {
            int lb = mask & (-mask);
            int idx = 0;
            while ((1 << idx) != lb) idx++;
            sum[mask] = sum[mask ^ lb] + w[idx];
        }
        int dp1[1024], dp2[1024];
        dp1[0] = 0;
        dp2[0] = 0;
        for (int mask = 1; mask <= full; mask++) {
            int best1 = INF, best2 = INF;
            for (int s = mask; s; s = (s - 1) & mask) {
                if (sum[s] <= C1 && dp1[mask ^ s] + 1 < best1) best1 = dp1[mask ^ s] + 1;
                if (sum[s] <= C2 && dp2[mask ^ s] + 1 < best2) best2 = dp2[mask ^ s] + 1;
            }
            dp1[mask] = best1;
            dp2[mask] = best2;
        }
        int ans = INF;
        for (int mask = 0; mask <= full; mask++) {
            int rest = full ^ mask;
            if (dp1[mask] >= INF || dp2[rest] >= INF) continue;
            int cost = dp1[mask] > dp2[rest] ? dp1[mask] : dp2[rest];
            if (cost < ans) ans = cost;
        }
        printf("Scenario #%d:\n%d\n\n", tc, ans);
    }
    return 0;
}
