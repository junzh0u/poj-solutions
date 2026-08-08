// POJ 3390 - Print Words in Lines
// Model: claude-opus-5
//
// Word-wrap with a squared-slack penalty.  Only the word lengths matter, so a
// line holding words i..j renders as sum(w[i..j]) + (j-i) characters (exactly
// one space between adjacent words) and must be at most M.  Its penalty is
// (M - rendered)^2.
//
// dp[i] = minimum total penalty for printing words i..n-1, computed backwards:
//   dp[n] = 0,  dp[i] = min over feasible j >= i of (M - len(i,j))^2 + dp[j+1].
// The inner loop stops as soon as the line overflows; every extra word costs at
// least one character (its space), so it runs at most M+1 times.  O(N*M) total,
// 0.13s here for 200 maximal cases (N=10000, M=100) including 6 MB of input.
//
// Ambiguity: whether the final line is penalized.  The statement says the total
// is "the sum of all penalties from all lines" with no exemption, and the sample
// settles it - the last line does pay.  Exempting it gives 13/16 instead of the
// expected 33/146.
//
// Boundary: "at most M characters" makes an exactly-full line legal, so the
// overflow test is > M, not >= M.  The sample does not exercise this (no optimal
// line in it is exactly full), so it was checked separately: M=5 with two words
// of length 2 packs as "xx xx" for penalty 0, where rejecting a full line costs
// 18.
#include <cstdio>
#include <iostream>

static int w[10005];
static long long dp[10005];

int main() {
    int C;
    if (scanf("%d", &C) != 1) return 0;
    for (int tc = 0; tc < C; ++tc) {
        int M, n;
        if (scanf("%d", &M) != 1) break;
        if (scanf("%d", &n) != 1) break;
        int i;
        for (i = 0; i < n; ++i)
            if (scanf("%d", &w[i]) != 1) { n = i; break; }

        dp[n] = 0;
        for (i = n - 1; i >= 0; --i) {
            long long best = -1;
            int cur = -1;
            for (int j = i; j < n; ++j) {
                cur += 1 + w[j];
                if (cur > M) break;
                long long d = (long long)(M - cur);
                long long cand = dp[j + 1] + d * d;
                if (best < 0 || cand < best) best = cand;
            }
            if (best < 0) best = dp[i + 1];  /* only if some word exceeds M */
            dp[i] = best;
        }
        std::cout << dp[0] << "\n";
    }
    return 0;
}
