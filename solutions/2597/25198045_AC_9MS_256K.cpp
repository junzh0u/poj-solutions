// POJ 2597 - Line Segment Erase
// Model: claude-sonnet-5
//
// For each test case, read M segments (endpoints S,E in unspecified
// order -- the discuss board notes some data deliberately reverses
// them -- canonicalize to l=min(S,E) < r=max(S,E)). Two segments
// "overlap" iff their closed intervals share more than a single point,
// i.e. max(l1,l2) < min(r1,r2); touching at a single endpoint is not
// an overlap (verified against the sample: [1,3] and [3,5] coexist).
// We need the maximum set of pairwise non-overlapping segments -- that
// determines the minimum erase count (M - maxset) -- and the number of
// distinct maximum sets, which equals the number of distinct erase
// choices since the erased set is exactly the complement of the kept
// set.
//
// Sort segments by (r ascending, l ascending). No two segments chosen
// together can share the same r (since l<r for every segment forces
// max(l_i,l_j) < r whenever r_i=r_j, i.e. they'd overlap), so every
// maximum independent set has a unique rightmost member once sorted by
// r. For two selected segments i,j with r_i<r_j, compatibility reduces
// to l_j >= r_i, and this holds simultaneously for every earlier
// selected segment (not just the immediate predecessor) whenever the
// whole set is pairwise compatible, so a chain DP over the sorted
// order with adjacent-pair transitions enumerates every maximum
// independent set exactly once.
//
// dp[i] = length of the longest compatible chain ending with segment i
// cnt[i] = number of such chains achieving dp[i]
// Answer: M - max(dp), and sum of cnt[i] over i attaining max(dp).
// Verified by hand against the sample: sorted order [1,3],[3,5],
// [4,6],[4,6],[8,9] gives three length-3 chains ending at the [8,9]
// segment -- matches "2 3".
//
// Input may have trailing blank lines after the last case (per the
// discuss board); reading with a scanf("%d",...) loop tolerates that
// automatically.

#include <cstdio>
#include <algorithm>
using namespace std;

struct Seg { int l, r; };

const int MAXN = 90;
Seg seg[MAXN];
long long dp[MAXN];
long long cnt[MAXN];

bool cmp(const Seg &a, const Seg &b) {
    if (a.r != b.r) return a.r < b.r;
    return a.l < b.l;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; i++) {
            int s, e;
            scanf("%d %d", &s, &e);
            seg[i].l = min(s, e);
            seg[i].r = max(s, e);
        }
        sort(seg, seg + n, cmp);
        long long best = 0, total = 0;
        for (int i = 0; i < n; i++) {
            dp[i] = 1;
            cnt[i] = 1;
            for (int j = 0; j < i; j++) {
                if (seg[j].r <= seg[i].l) {
                    if (dp[j] + 1 > dp[i]) {
                        dp[i] = dp[j] + 1;
                        cnt[i] = cnt[j];
                    } else if (dp[j] + 1 == dp[i]) {
                        cnt[i] += cnt[j];
                    }
                }
            }
            if (dp[i] > best) {
                best = dp[i];
                total = cnt[i];
            } else if (dp[i] == best) {
                total += cnt[i];
            }
        }
        printf("%lld %lld\n", (long long)n - best, total);
    }
    return 0;
}
