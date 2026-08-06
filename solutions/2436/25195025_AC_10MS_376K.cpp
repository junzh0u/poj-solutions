// POJ 2436 - Disease Management
// Model: claude-opus-5
//
// Approach: D <= 15, so encode each cow's disease set as a bitmask over D bits.
// cnt[m] = number of cows whose disease set is exactly m.  A subset-sum (SOS)
// DP over the bits turns that into f[S] = number of cows whose disease set is a
// subset of S, in O(D * 2^D).  The answer is max over all S with popcount(S)<=K
// of f[S].  Total work ~ 15 * 32768 plus the input scan, far inside the limits.
//
// Notes on the statement: diseases are numbered 1..D and a cow's list may be
// empty (d_i = 0), so a cow with no diseases has mask 0 and is countable under
// every S -- the answer is therefore never smaller than the number of healthy
// cows.  K <= D is guaranteed, so masks of popcount exactly min(K,D) already
// dominate; the loop still tests popcount <= K, which is the same maximum and
// needs no special case.  Input is read until EOF in case the judge data holds
// more than one test case.

#include <cstdio>

int cnt[1 << 15];

int main() {
    int n, d, k;
    while (scanf("%d %d %d", &n, &d, &k) == 3) {
        int total = 1 << d;
        int i, j;
        for (i = 0; i < total; ++i) cnt[i] = 0;

        for (i = 0; i < n; ++i) {
            int c, x, mask = 0;
            if (scanf("%d", &c) != 1) c = 0;
            for (j = 0; j < c; ++j) {
                if (scanf("%d", &x) != 1) x = 0;
                if (x >= 1 && x <= d) mask |= 1 << (x - 1);
            }
            ++cnt[mask];
        }

        /* subset-sum DP: cnt[S] becomes the number of cows with set subset of S */
        for (i = 0; i < d; ++i) {
            int bit = 1 << i;
            for (j = 0; j < total; ++j)
                if (j & bit) cnt[j] += cnt[j ^ bit];
        }

        int best = 0;
        for (i = 0; i < total; ++i) {
            int pc = 0, t = i;
            while (t) { t &= t - 1; ++pc; }
            if (pc <= k && cnt[i] > best) best = cnt[i];
        }
        printf("%d\n", best);
    }
    return 0;
}
