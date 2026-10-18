// POJ 2743 - Mobile Computing
// Model: gpt-6-astra
// Enumerate every ordered binary mobile for each subset of stones.
// Store both extrema relative to its suspension point. Either child may
// extend beyond the other, since overlapping sub-mobiles are permitted.
// Discard widths at least the room width: adding a parent cannot shrink them.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Span {
    double left, right;
    Span(double l, double r) : left(l), right(r) {}
};
int main() {
    int tests;
    scanf("%d", &tests);
    while (tests--) {
        double room;
        int n, w[6], sum[64] = {0};
        scanf("%lf%d", &room, &n);
        for (int i = 0; i < n; ++i) scanf("%d", &w[i]);
        vector<Span> dp[64];
        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int i = 0; i < n; ++i)
                if (mask & (1 << i)) sum[mask] += w[i];
            if ((mask & (mask - 1)) == 0) {
                dp[mask].push_back(Span(0, 0));
                continue;
            }
            for (int a = (mask - 1) & mask; a; a = (a - 1) & mask) {
                int b = mask ^ a;
                double dl = double(sum[b]) / sum[mask];
                double dr = double(sum[a]) / sum[mask];
                for (size_t i = 0; i < dp[a].size(); ++i)
                    for (size_t j = 0; j < dp[b].size(); ++j) {
                        double l = max(dp[a][i].left + dl, dp[b][j].left - dr);
                        double r = max(dp[a][i].right - dl, dp[b][j].right + dr);
                        if (l + r < room) dp[mask].push_back(Span(l, r));
                    }
            }
        }
        double answer = -1;
        const vector<Span>& all = dp[(1 << n) - 1];
        for (size_t i = 0; i < all.size(); ++i)
            answer = max(answer, all[i].left + all[i].right);
        if (answer < 0) puts("-1");
        else printf("%.16f\n", answer);
    }
    return 0;
}
