// POJ 2744 - Atomic Car Race
// Model: gpt-6-astra
// Precompute travel times for each tire age, then minimize over the
// previous tire-change checkpoint. The start incurs no change cost;
// a change is charged on departure, so the goal incurs none.
// Each kilometer uses its starting age x, not the ending age x+1.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        vector<int> a(n + 1, 0);
        for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
        double b, v, e, f;
        int r;
        scanf("%lf%d%lf%lf%lf", &b, &r, &v, &e, &f);
        vector<double> t(a[n] + 1, 0.0), dp(n + 1, 0.0);
        for (int x = 0; x < a[n]; ++x) {
            double speed = x >= r ? v - e * (x - r) : v - f * (r - x);
            t[x + 1] = t[x] + 1.0 / speed;
        }
        for (int i = 1; i <= n; ++i) {
            dp[i] = t[a[i]];
            for (int j = 1; j < i; ++j)
                dp[i] = min(dp[i], dp[j] + b + t[a[i] - a[j]]);
        }
        printf("%.4f\n", dp[n]);
    }
    return 0;
}
