// POJ 2772 - Up the Stairs
// Model: claude-opus-5
//
// A hand-over happens at a single point, so a box never changes height when
// it changes hands: every box, once picked up, rises at exactly 1 floor per
// minute until it reaches the penthouse.  The persons are therefore
// interchangeable -- a meeting is indistinguishable from the two walking
// through each other -- and the system is N independent shuttles bouncing
// between floor 0 and floor F with period 2F.
//
// Shuttle i first reaches the ground floor to collect a fresh box at
//   t_i = f_i          if it is empty-handed (it walks straight down), or
//   t_i = 2F - f_i     if it holds a box (deliver first, then walk down),
// and again every 2F minutes after that.  Since t_i lies in [0, 2F], sorting
// the t_i already orders the whole merged pickup schedule round by round, so
// the B-th ground box (1-indexed) is collected at t_sorted[(B-1)%N] +
// 2F*((B-1)/N) and delivered F minutes later.
//
// The B boxes still on the ground are the last to arrive: a carried box is
// delivered at F - f_i <= F, while the earliest ground box needs at least F,
// so the boxes already in hand never decide the answer and are not counted
// into B (confirmed on the discuss board).  That distinction is the one the
// official sample cannot see -- a variant where a box-holder skips the walk
// back down reproduces both sample lines -- so it was settled by simulating
// the literal statement (persons keeping their identity, hand-overs only
// when two of them stand on the same point) over all 47040 configurations
// with N<=3, F<=5, B<=12, plus 4000 random ones up to N=6, F=12, B=40.
//
// The largest answer under the stated bounds is 2*1000*999999 + 2000 + 1000
// = 2000001000, which still fits in a signed 32-bit int.

#include <cstdio>
#include <algorithm>

int t[1005];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n, F, B;
        scanf("%d %d %d", &n, &F, &B);
        for (int i = 0; i < n; ++i) {
            int f, b;
            scanf("%d %d", &f, &b);
            t[i] = b ? (2 * F - f) : f;
        }
        std::sort(t, t + n);
        int k = (B - 1) / n;
        int r = (B - 1) % n;
        printf("%d\n", k * 2 * F + t[r] + F);
    }
    return 0;
}
