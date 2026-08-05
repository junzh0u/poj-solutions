// POJ 3616 - Milking Time
//
// Weighted interval scheduling with a mandatory rest gap.
//
// Each of the M (<= 1000) intervals is [s, e) with a flat payoff w -- the
// efficiency is the total gallons for the whole interval, not a per-hour rate,
// and an interval must be taken whole or not at all. N is irrelevant to the
// answer beyond bounding the coordinates.
//
// The one thing the statement only says obliquely: "Farmer John starts and
// stops milking at the beginning of the starting hour and ending hour", so an
// interval occupies hours s..e-1 and the cow is free again at hour e; the R
// hours of rest are counted from that end time. Interval j may therefore
// precede interval i exactly when
//
//     end_j + R <= start_i
//
// (an interval starting precisely at end_j + R is legal). The sample pins this
// down: 24 over [3,6) then 19 over [10,12) with R = 2 needs 10 >= 6 + 2, and
// gives the answer 43.
//
// Sort by end time and run the obvious DP: dp[i] = w_i + max over all earlier-
// finishing j satisfying the gap condition of dp[j]. The O(M^2) scan is 10^6
// comparisons at the limits, a few milliseconds, so there is no need for the
// sorted-by-end binary search. The answer is max dp[i]; it is bounded by
// 1000 * 10^6 = 10^9, which still fits in 32 bits but is accumulated in
// long long for margin.

#include <cstdio>
#include <iostream>
#include <algorithm>

struct Iv {
    int s, e, w;
};

static bool byEnd(const Iv &a, const Iv &b) {
    if (a.e != b.e) return a.e < b.e;
    return a.s < b.s;
}

int main() {
    int n, m, r;
    if (scanf("%d %d %d", &n, &m, &r) != 3) return 0;
    (void)n;
    static Iv iv[1005];
    for (int i = 0; i < m; ++i)
        scanf("%d %d %d", &iv[i].s, &iv[i].e, &iv[i].w);
    std::sort(iv, iv + m, byEnd);

    static long long dp[1005];
    long long best = 0;
    for (int i = 0; i < m; ++i) {
        long long cur = 0;
        for (int j = 0; j < i; ++j) {
            /* j may precede i iff j finishes at least R hours before i
             * starts: end_j + R <= start_i */
            if (iv[j].e + r <= iv[i].s && dp[j] > cur) cur = dp[j];
        }
        dp[i] = cur + iv[i].w;
        if (dp[i] > best) best = dp[i];
    }
    std::cout << best << std::endl;
    return 0;
}
