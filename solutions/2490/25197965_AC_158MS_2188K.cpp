// POJ 2490 - Pimp My Ride
// Model: claude-sonnet-5
// Approach: bitmask DP over which jobs are done. dp[mask] = min cost to
// complete exactly the jobs in mask (in some order). Transition picks the
// last job i added: dp[mask] = min over i in mask of
//   dp[mask ^ (1<<i)] + price[i] + sum_{j in mask, j != i} surcharge[i][j]
// since the surcharge for job i counts every job already completed before
// it, not just the immediately preceding one (confirmed on the discuss
// board: "when doing job i you must add up the surcharge from ALL
// previously completed jobs, not just the one right before it").
// The inner sum is precomputed with a submask running sum
// addS[i][mask] = sum_{j in mask} surcharge[i][j], built by peeling off the
// lowest set bit, so the whole DP runs in O(n * 2^n).
// Ambiguity: none really — sample 1 pins down which index (i-th vs j-th
// column) is the base price and which is the surcharge; verified by hand
// against both sample scenarios (30 and 42).
#include <cstdio>
#include <cstring>

typedef long long ll;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        int n;
        scanf("%d", &n);
        static int price[14];
        static int sur[14][14];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int v;
                scanf("%d", &v);
                if (j == i) price[i] = v;
                else sur[i][j] = v;
            }
        }
        int full = 1 << n;
        // addS[i][mask] = sum_{j in mask} sur[i][j]
        static ll addS[14][1 << 14];
        for (int i = 0; i < n; ++i) {
            addS[i][0] = 0;
            for (int mask = 1; mask < full; ++mask) {
                int low = mask & (-mask);
                int j = 0;
                while ((low >> j) != 1) ++j;
                addS[i][mask] = addS[i][mask ^ low] + sur[i][j];
            }
        }
        static ll dp[1 << 14];
        dp[0] = 0;
        for (int mask = 1; mask < full; ++mask) {
            ll best = -1;
            for (int i = 0; i < n; ++i) {
                if (!(mask & (1 << i))) continue;
                int prev = mask ^ (1 << i);
                ll cost = dp[prev] + price[i] + addS[i][prev];
                if (best < 0 || cost < best) best = cost;
            }
            dp[mask] = best;
        }
        printf("Scenario #%d:\n", tc);
        printf("You have officially been pimped for only $%lld\n\n", dp[full - 1]);
    }
    return 0;
}
