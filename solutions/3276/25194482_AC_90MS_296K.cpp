// POJ 3276 - Face The Right Way
//
// N cows in a row, each facing F or B.  The machine flips a contiguous block of
// exactly K cows, K fixed for the whole run.  Find the K minimising the number
// of operations M, and report that K with its M.
//
// For a fixed K the greedy is forced: sweep left to right, and when cow i still
// faces backward, the only flip that can still touch it starts exactly at i --
// every block starting further left is already behind us, and blocks starting
// further right do not cover i.  So flip there or fail.  It fails only when
// i + K > N (the block would run off the end); K = 1 never fails, so an answer
// always exists.  Flip order is irrelevant and flipping one start twice cancels,
// so this greedy set is also minimal.
//
// Applying the flips naively is O(NK); instead diff[] records where each flip
// stops covering, and the running counter `cur` holds how many flips currently
// cover position i.  Only its parity matters.  That makes each K an O(N) pass
// and the whole sweep over K = 1..N an O(N^2) = 25M-step scan: 90MS here.
//
// Tie-break: the statement asks for the minimum K among those achieving the
// minimum M, so the comparison below is strict (`ops < bestM`) and K ascends --
// the first K reaching a given M wins.  This matters for an all-forward row,
// where every K costs 0 operations and the answer is "1 0".

#include <cstdio>

static int n;
static int a[5005];      /* 1 = backward (needs flipping), 0 = forward */
static int diff[5006];   /* difference array of flip starts */

int main() {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) {
        char buf[8];
        if (scanf("%s", buf) != 1) return 0;
        a[i] = (buf[0] == 'B') ? 1 : 0;
    }

    int bestK = 1, bestM = -1;
    for (int k = 1; k <= n; ++k) {
        for (int i = 0; i <= n; ++i) diff[i] = 0;
        int cur = 0, ops = 0;
        bool ok = true;
        for (int i = 0; i < n; ++i) {
            cur += diff[i];
            int state = a[i] ^ (cur & 1);
            if (state) {
                if (i + k > n) { ok = false; break; }
                ++ops;
                ++cur;
                --diff[i + k];   /* the flip stops covering position i+k */
            }
        }
        if (ok && (bestM < 0 || ops < bestM)) { bestM = ops; bestK = k; }
    }

    printf("%d %d\n", bestK, bestM);
    return 0;
}
