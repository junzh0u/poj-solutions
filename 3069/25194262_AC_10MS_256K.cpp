// POJ 3069 - Saruman's Army
//
// Troops sit on a line; a palantir has range R and must be *carried by a
// troop*, so its position has to coincide with some x[i].  Minimise the
// number of palantirs so every troop is within R of one.
//
// Greedy, left to right.  Sort the positions.  The leftmost troop s that is
// not yet covered must be covered by a palantir sitting on a troop in
// [s, s+R] (anything left of s is pointless, anything right of s+R misses
// s).  Pushing that palantir as far right as possible is never worse, so
// pick the last troop p <= s+R as the carrier, then skip every troop up to
// p+R and repeat.  O(n log n) for the sort, one linear sweep after it.
//
// Statement notes:
//  - Positions are not promised to be sorted or distinct, hence the sort;
//    duplicates fall out for free (they are covered at distance 0).
//  - R = 0 is allowed: a palantir then covers exactly the troops standing on
//    its own position, so the answer is the number of distinct positions.
//  - Multiple test cases until the sentinel line "-1 -1".

#include <cstdio>
#include <algorithm>

int main() {
    int R, n;
    while (scanf("%d %d", &R, &n) == 2) {
        if (R == -1 && n == -1) break;
        static int x[1005];
        for (int i = 0; i < n; ++i) scanf("%d", &x[i]);
        std::sort(x, x + n);
        int i = 0, cnt = 0;
        while (i < n) {
            /* leftmost uncovered troop */
            int s = x[i];
            /* rightmost troop that can still host a palantir covering s */
            int j = i;
            while (j < n && x[j] <= s + R) ++j;
            --j;                    /* j is the palantir carrier */
            int p = x[j];
            ++cnt;
            /* skip everything this palantir covers */
            while (j < n && x[j] <= p + R) ++j;
            i = j;
        }
        printf("%d\n", cnt);
    }
    return 0;
}
