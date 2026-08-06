// POJ 2549 - Sumsets
//
// Given a set S (n <= 1000, values in [-536870912, 536870911], and the
// statement guarantees they are pairwise distinct), find the largest d in S
// with a + b + c = d for a, b, c, d four distinct elements.
//
// Meet in the middle on a + b = d - c: tabulate every unordered pair sum
// a + b together with the two indices it came from (n*(n-1)/2 <= 499500
// entries) and sort by sum. Then walk d from the largest element downwards
// and, for each c != d, look up the target d - c among the pair sums; the
// first d that yields a hit is the answer, so the search stops there.
//
// The subtlety is that a hit is only valid when the stored pair shares no
// index with (d, c) -- otherwise an element gets reused. Because the values
// are distinct, a given sum value is reached by at most one pair containing
// index d and at most one containing index c, so among any three pairs with
// that sum at least one is index-disjoint from {d, c}: inspecting the first
// four equal-sum entries is enough, no matter how many pairs share the sum.
// The input is deduplicated on the way in so that argument holds even if the
// judge data ever violates the distinctness promise; n < 4 after dedup is an
// immediate "no solution".
//
// The lookup is bucketed rather than a plain binary search over the whole
// array: sums are indexed by a shifted offset from the minimum, so a query
// touches one small contiguous run. That matters because the worst case (no
// solution, n = 1000) does all n*(n-1) ~ 10^6 lookups, and the time limit is
// 1000MS -- the bucket index cut a 20-set stress from 0.82s to 0.18s locally.
//
// a + b and d - c both fit in int: |values| < 2^29, so the sums stay under
// 2^30 in magnitude.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1000;
const int MAXP = MAXN * (MAXN - 1) / 2;
const int MAXB = 1 << 20;

struct P {
    int s;
    short i, j;
};

int v[MAXN];
P pr[MAXP];
int sums[MAXP];
int start_[MAXB + 2];

struct CmpP {
    bool operator()(const P &a, const P &b) const { return a.s < b.s; }
};

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int k;
        for (k = 0; k < n; k++) scanf("%d", &v[k]);
        sort(v, v + n);
        int m = (int)(unique(v, v + n) - v);
        if (m < 4) {
            printf("no solution\n");
            continue;
        }

        int cnt = 0;
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++) {
                pr[cnt].s = v[i] + v[j];
                pr[cnt].i = (short)i;
                pr[cnt].j = (short)j;
                cnt++;
            }
        sort(pr, pr + cnt, CmpP());
        for (int t = 0; t < cnt; t++) sums[t] = pr[t].s;

        /* bucket index over the sorted sums so a lookup touches a short
           contiguous run instead of binary searching the whole array */
        int minS = sums[0], maxS = sums[cnt - 1];
        unsigned range = (unsigned)(maxS - minS);
        int shift = 0;
        while ((range >> shift) >= (unsigned)(MAXB - 1)) shift++;
        int nb = (int)(range >> shift) + 1;
        memset(start_, 0, sizeof(int) * (nb + 2));
        for (int t = 0; t < cnt; t++)
            start_[(int)(((unsigned)(sums[t] - minS)) >> shift) + 1]++;
        for (int b = 0; b < nb + 1; b++) start_[b + 1] += start_[b];

        bool found = false;
        int ansIdx = -1;
        for (int di = m - 1; di >= 0 && !found; di--) {
            for (int ci = 0; ci < m && !found; ci++) {
                if (ci == di) continue;
                int target = v[di] - v[ci];
                if (target < minS || target > maxS) continue;
                int b = (int)(((unsigned)(target - minS)) >> shift);
                int lo = start_[b], hi = start_[b + 1];
                if (lo == hi) continue;
                int p = (int)(lower_bound(sums + lo, sums + hi, target) - sums);
                /* the values are distinct, so at most one stored pair contains
                   index di and at most one contains ci: among any three pairs
                   with this sum one is index-disjoint from {di, ci} */
                for (int t = 0; t < 4 && p + t < hi && sums[p + t] == target; t++) {
                    int a = pr[p + t].i, c = pr[p + t].j;
                    if (a != di && a != ci && c != di && c != ci) {
                        found = true;
                        ansIdx = di;
                        break;
                    }
                }
            }
        }
        if (found)
            printf("%d\n", v[ansIdx]);
        else
            printf("no solution\n");
    }
    return 0;
}
