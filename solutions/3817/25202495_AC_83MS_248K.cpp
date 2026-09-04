// POJ 3817 - Robot Challenge
// Model: claude-sonnet-5
//
// Approach: The robot must visit stop-points in order: (0,0) [no stop
// counted], target 1..N in order (each may be skipped), and finally
// (100,100) [always visited, 1s stop]. Between two consecutive *actual*
// stops j -> i, the robot travels the straight-line distance, pays a 1s
// stop at i, and pays the penalty of every target strictly between j and
// i that was skipped. This is a simple O(N^2) DP over which targets are
// actually stopped at:
//   dp[0] = 0                      (start at (0,0), not a counted stop)
//   dp[i] = min_{j<i} dp[j] + dist(j,i) + 1 + (prefix[i-1] - prefix[j])
// where prefix[k] is the sum of penalties of targets 1..k, and i ranges
// over 1..N+1 with point N+1 = (100,100), penalty 0 (never skippable).
// Answer is dp[N+1]. N <= 1000 so O(N^2) (~10^6) per test case is fine;
// with "several test cases" total work stays comfortable within the 1s
// limit even for a handful of near-max cases.
//
// No statement ambiguity found; verified exactly against all three
// sample outputs (143.421 / 237.716 / 154.421) with a straightforward
// Python re-implementation before porting to C++.

#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static double X[1002], Y[1002];
        static double pen[1002];
        static double prefix[1002];
        X[0] = 0.0; Y[0] = 0.0; pen[0] = 0.0;
        for (int i = 1; i <= n; ++i) {
            int x, y, p;
            scanf("%d %d %d", &x, &y, &p);
            X[i] = x; Y[i] = y; pen[i] = p;
        }
        X[n+1] = 100.0; Y[n+1] = 100.0; pen[n+1] = 0.0;

        prefix[0] = 0.0;
        for (int i = 1; i <= n; ++i) prefix[i] = prefix[i-1] + pen[i];
        prefix[n+1] = prefix[n];

        static double dp[1002];
        dp[0] = 0.0;
        for (int i = 1; i <= n+1; ++i) {
            double best = -1.0;
            for (int j = 0; j < i; ++j) {
                double dx = X[i]-X[j], dy = Y[i]-Y[j];
                double d = sqrt(dx*dx+dy*dy);
                double val = dp[j] + d + 1.0 + (prefix[i-1]-prefix[j]);
                if (best < 0 || val < best) best = val;
            }
            dp[i] = best;
        }
        printf("%.3f\n", dp[n+1]);
    }
    return 0;
}
