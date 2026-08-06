// POJ 1948 - Triangular Pastures
// Model: claude-sonnet-5
// Approach: partition all N fence segments into three non-empty groups (the
// three sides) to maximize the enclosed triangle's area. Track, via a
// bitset subset-sum DP over item index, which pairs (sumA, sumB) are
// reachable when every segment is assigned to side A, side B, or (implicitly,
// by not touching the running sums) side C; sumC = total - sumA - sumB falls
// out automatically since total is fixed in advance. For a fixed item of
// length L, processing sums a from high to low (classic 0/1 knapsack order)
// with dp[a] |= dp[a-L] (assign to A) and dp[a] |= (old dp[a] << L) (assign
// to B) — using a snapshot of dp[a] taken before either update this item, so
// neither transition reuses the same item twice — correctly folds in the
// choice of leaving it on side C (dp[a] unchanged) as well.
// Once all reachable (a,b) pairs are known, maximize Heron's formula in
// integer form: 16*Area^2 = total*(total-2a)*(total-2b)*(total-2c), valid
// exactly when 2a,2b,2c < total (equivalent to the strict triangle
// inequality on all three sides, since a+b+c=total). Maximizing this
// integer product P is equivalent to maximizing Area (sqrt is monotonic),
// so the search stays exact until the very end. The required output is
// floor(100*Area) = floor(sqrt(625*P)), computed with an exact integer
// square root (long double estimate + adjustment) to avoid truncation
// errors near integer boundaries.
#include <cstdio>
#include <cmath>
#include <bitset>
using namespace std;

const int MAXS = 1601;

int len_[45];

int main() {
    int n;
    scanf("%d", &n);
    int total = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &len_[i]);
        total += len_[i];
    }

    static bitset<MAXS> dp[MAXS];
    dp[0][0] = 1;

    for (int i = 0; i < n; i++) {
        int L = len_[i];
        for (int a = total; a >= 0; a--) {
            bitset<MAXS> oldRow = dp[a];
            if (a >= L) dp[a] |= dp[a - L];
            dp[a] |= (oldRow << L);
        }
    }

    long long best = -1;
    for (int a = 1; a < total; a++) {
        if (2 * a >= total) continue;
        int bLimit = total - a; // c = total-a-b >= 1 => b <= total-a-1
        for (int b = 1; b < bLimit; b++) {
            if (2 * b >= total) continue;
            int c = total - a - b;
            if (2 * c >= total) continue;
            if (!dp[a][b]) continue;
            long long P = (long long)total * (total - 2 * a) * (total - 2 * b) * (total - 2 * c);
            if (P > best) best = P;
        }
    }

    if (best < 0) {
        printf("-1\n");
    } else {
        long long Q = 625LL * best;
        long long r = (long long)sqrtl((long double)Q);
        while (r * r > Q) r--;
        while ((r + 1) * (r + 1) <= Q) r++;
        printf("%lld\n", r);
    }

    return 0;
}
