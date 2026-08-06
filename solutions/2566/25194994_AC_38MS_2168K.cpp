// POJ 2566 - Bound Found
// Model: claude-sonnet-5
// Approach: compute prefix sums S[0..n] and sort them by value. |S[j]-S[i]|
// for any i != j equals the absolute sum of the subrange between the two
// prefix positions, so after sorting, a two-pointer sweep (lo, hi) over the
// sorted prefix sums finds, for each query t, the pair whose non-negative
// difference is closest to t in O(n) per query (classic "closest pair to a
// target" two-pointer, confirmed as the intended technique on the problem's
// discuss board). Sums must be kept in 64-bit (board also flags this: values
// up to 1e5 * 1e4 = 1e9 in magnitude, differences up to 2e9, overflowing
// 32-bit int). No statement ambiguity: it is Special Judge, so any range
// achieving the closest absolute sum is accepted, ties included.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

struct Node {
    long long val;
    int idx;
    bool operator<(const Node& o) const { return val < o.val; }
};

Node nodes[MAXN];
int a[MAXN];

int main() {
    int n, k;
    while (scanf("%d %d", &n, &k) == 2) {
        if (n == 0 && k == 0) break;
        for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
        long long sum = 0;
        nodes[0].val = 0; nodes[0].idx = 0;
        for (int i = 1; i <= n; ++i) {
            sum += a[i];
            nodes[i].val = sum;
            nodes[i].idx = i;
        }
        sort(nodes, nodes + n + 1);
        int m = n + 1;
        for (int q = 0; q < k; ++q) {
            long long t;
            scanf("%lld", &t);
            int lo = 0, hi = 1;
            long long bestDiff = 0, bestErr = -1;
            int bestL = 1, bestU = 1;
            bool found = false;
            while (hi < m) {
                long long diff = nodes[hi].val - nodes[lo].val;
                long long err = diff > t ? diff - t : t - diff;
                if (!found || err < bestErr) {
                    found = true;
                    bestErr = err;
                    bestDiff = diff;
                    bestL = min(nodes[lo].idx, nodes[hi].idx) + 1;
                    bestU = max(nodes[lo].idx, nodes[hi].idx);
                }
                if (diff == t) break;
                if (diff < t) {
                    ++hi;
                } else {
                    ++lo;
                    if (lo == hi) ++hi;
                }
            }
            printf("%lld %d %d\n", bestDiff, bestL, bestU);
        }
    }
    return 0;
}
