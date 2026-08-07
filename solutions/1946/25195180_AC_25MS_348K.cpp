// POJ 1946 - Cow Cycling
// Model: claude-opus-5
//
// Key observation: a cow that merely drafts spends exactly x energy in a
// minute the pack covers x laps, so its total draft cost equals the distance
// it has travelled with the pack.  Hence when the k-th leader takes over at
// lap j0, every cow still in the pack has spent exactly j0 energy, and the
// new leader can afford at most E - j0 more.  While it leads over laps
// x_1..x_t it spends sum(x_s^2), so the block from lap j0 to lap j is legal
// iff min sum of squares of t positive integers summing to (j - j0) is
// <= E - j0.  That minimum is the balanced split (r parts of q+1 and t-r
// parts of q, where q = d/t, r = d%t).
//
// So the race decomposes into at most N contiguous leader blocks and
//   f[i][j] = min time to reach lap j with i cows having led,
//   f[i][j] = min over j0 < j of f[i-1][j0] + minT(j0, j),
// with minT the smallest t whose balanced split fits the budget E - j0.
// Answer = min over i of f[i][D], or 0 when D is unreachable (i.e. D > E,
// since a lone cow riding at 1 lap/minute spends exactly D).
//
// Verified against a BFS brute force written from the statement's own words
// (arbitrary per-minute leader choice, cows dropping out freely, so leader
// blocks need not be contiguous) over every N<=6, E<=26, D<=18 combination
// tried, plus the discuss board's cases 16 88 34 / 17 78 45 / 18 100 23 /
// 19 99 45 / 20 100 100 -> 5 7 3 6 19.
//
// Note the input order is N, E, D (energy before distance).  Input is read
// to EOF so several data sets on separate lines are handled too.

#include <cstdio>
#include <algorithm>
using namespace std;

const int INF = 1000000000;

int f[21][105];

int main() {
    int N, E, D;
    while (scanf("%d %d %d", &N, &E, &D) == 3) {
        for (int i = 0; i <= N; i++)
            for (int j = 0; j <= D; j++) f[i][j] = INF;
        f[0][0] = 0;
        for (int i = 1; i <= N; i++) {
            for (int j0 = 0; j0 < D; j0++) {
                if (f[i - 1][j0] >= INF) continue;
                int budget = E - j0;
                if (budget <= 0) continue;
                for (int j = j0 + 1; j <= D; j++) {
                    int d = j - j0;
                    for (int t = 1; t <= d; t++) {
                        int q = d / t, r = d % t;
                        int s = r * (q + 1) * (q + 1) + (t - r) * q * q;
                        if (s <= budget) {
                            if (f[i - 1][j0] + t < f[i][j]) f[i][j] = f[i - 1][j0] + t;
                            break;
                        }
                    }
                }
            }
        }
        int best = INF;
        for (int i = 1; i <= N; i++) best = min(best, f[i][D]);
        printf("%d\n", best >= INF ? 0 : best);
    }
    return 0;
}
