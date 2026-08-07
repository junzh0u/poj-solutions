// POJ 2355 - Railway tickets
// Model: claude-sonnet-5
// Approach: stations lie on a line with strictly increasing positions.
// Ticket price between two stations depends only on distance, in three
// tiers (<=L1 -> C1, <=L2 -> C2, <=L3 -> C3, C1<C2<C3, L1<L2<L3), and a
// direct ticket only exists for distance <= L3. Minimal cost from station
// s to station t (s<t after sorting the two given endpoints, since the
// board warns the given pair may arrive with a>b) only ever needs forward
// hops, because price is nondecreasing in distance so backtracking is
// never beneficial. dp[i] = min cost to reach i from s; dp is nondecreasing
// in i (reachable via extending any optimal prefix), so for each tier the
// cheapest predecessor within that tier's distance window is always the
// leftmost (farthest-back) station in that window. Three pointers, each
// monotonically advancing as i increases, track the leftmost index within
// L1, L2, L3 respectively, giving an O(N) sweep from s to t. A predecessor
// whose true distance is smaller than the tier being tested is still a
// legal (if mislabeled/inflated) candidate, so mixing tiers never produces
// an answer below the true optimum; it is validated separately against an
// O(N^2) brute-force DP on randomized tests.
#include <cstdio>
#include <algorithm>
using namespace std;

static const long long INF = (long long)4e18;

int main() {
    long long L1, L2, L3, C1, C2, C3;
    scanf("%lld %lld %lld %lld %lld %lld", &L1, &L2, &L3, &C1, &C2, &C3);
    int n;
    scanf("%d", &n);
    int a, b;
    scanf("%d %d", &a, &b);
    static long long pos[10005];
    pos[1] = 0;
    for (int i = 2; i <= n; ++i) scanf("%lld", &pos[i]);

    int s = min(a, b), t = max(a, b);
    static long long dp[10005];
    dp[s] = 0;

    int p1 = s, p2 = s, p3 = s;
    for (int i = s + 1; i <= t; ++i) {
        while (pos[i] - pos[p1] > L1) ++p1;
        while (pos[i] - pos[p2] > L2) ++p2;
        while (pos[i] - pos[p3] > L3) ++p3;

        long long best = INF;
        if (p1 < i) {
            long long cand = dp[p1] + C1;
            if (cand < best) best = cand;
        }
        if (p2 < i) {
            long long cand = dp[p2] + C2;
            if (cand < best) best = cand;
        }
        if (p3 < i) {
            long long cand = dp[p3] + C3;
            if (cand < best) best = cand;
        }
        dp[i] = best;
    }
    printf("%lld\n", dp[t]);
    return 0;
}
