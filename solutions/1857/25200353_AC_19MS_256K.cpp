// POJ 1857 - To Europe! To Europe!
// Model: gpt-5.6-terra
// Dynamic programming over the fixed queue order.  dp[i] is the least time
// for the first i vehicles; enumerate every load-feasible final contiguous
// group and add its slowest vehicle's crossing time.
// The discussion board confirms that vehicles must not be reordered.

#include <cstdio>
#include <algorithm>

int main() {
    int limit, length, n;
    while (std::scanf("%d%d%d", &limit, &length, &n) == 3 && (limit || length || n)) {
        int weight[1001], speed[1001];
        for (int i = 1; i <= n; ++i) std::scanf("%d%d", &weight[i], &speed[i]);

        double dp[1001];
        dp[0] = 0.0;
        for (int i = 1; i <= n; ++i) {
            dp[i] = 1e100;
            int total = 0, slowest = 1000000000;
            for (int j = i; j >= 1; --j) {
                total += weight[j];
                if (total > limit) break;
                slowest = std::min(slowest, speed[j]);
                dp[i] = std::min(dp[i], dp[j - 1] + 60.0 * length / slowest);
            }
        }
        std::printf("%.1f\n", dp[n]);
    }
    return 0;
}
