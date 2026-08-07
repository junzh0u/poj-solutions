// POJ 3612 - Telephone Wire
// Model: claude-sonnet-5
// DP over pole heights h in 1..100: dp[h] = min total cost so far with the
// current pole grown to height h. Growth is one-directional (X in "add an
// integer X meters" is nonnegative), so h < height[i] is infeasible and
// dp[h] is +INF there. Transition for h >= height[i]:
//   dp_new[h] = (h - height[i])^2 + min_hp( dp_old[hp] + C*|h - hp| )
// The inner min over hp is split at h: for hp<=h it is
// min(dp_old[hp] - C*hp) + C*h, a running prefix minimum; for hp>=h it is
// min(dp_old[hp] + C*hp) - C*h, a running suffix minimum. Both are O(100)
// per pole via one prefix and one suffix pass, giving O(N*100) total
// instead of the naive O(N*100*100) triple loop the discuss board
// (message 134630/137523) reports TLEing.
// It is never beneficial to grow a pole above height 100 (the maximum any
// original pole can have): every neighbor's height is <=100, so raising a
// pole above that only increases both (h-height)^2 and every wire penalty
// touching it, never decreasing anything. Heights 1..100 therefore dominate
// the whole search space and no larger range needs to be considered.
// Discuss board message 137201 (11 upvotes) reports the judge input holds
// multiple datasets read to EOF, despite the statement not mentioning it;
// the while(scanf) loop is a harmless no-op if there is only one dataset.
// Message 145097 reports the answer can exceed a 32-bit int, so dp and the
// answer are accumulated in long long.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXH = 100;
const long long INF = (long long)1 << 50;
int height_[100005];
long long dp[MAXH + 2];
long long ndp[MAXH + 2];
long long pref[MAXH + 2];
long long suf[MAXH + 2];

int main() {
    int n, c;
    while (scanf("%d %d", &n, &c) == 2) {
        for (int i = 1; i <= n; ++i) scanf("%d", &height_[i]);
        for (int h = 1; h <= MAXH; ++h) {
            if (h < height_[1]) {
                dp[h] = INF;
            } else {
                long long d = h - height_[1];
                dp[h] = d * d;
            }
        }
        for (int i = 2; i <= n; ++i) {
            pref[0] = INF;
            for (int h = 1; h <= MAXH; ++h) {
                long long val = dp[h] - (long long)c * h;
                pref[h] = min(pref[h - 1], val);
            }
            suf[MAXH + 1] = INF;
            for (int h = MAXH; h >= 1; --h) {
                long long val = dp[h] + (long long)c * h;
                suf[h] = min(suf[h + 1], val);
            }
            for (int h = 1; h <= MAXH; ++h) {
                if (h < height_[i]) {
                    ndp[h] = INF;
                    continue;
                }
                long long best = min(pref[h] + (long long)c * h, suf[h] - (long long)c * h);
                long long d = h - height_[i];
                ndp[h] = d * d + best;
            }
            for (int h = 1; h <= MAXH; ++h) dp[h] = ndp[h];
        }
        long long ans = dp[1];
        for (int h = 2; h <= MAXH; ++h) if (dp[h] < ans) ans = dp[h];
        printf("%lld\n", ans);
    }
    return 0;
}
