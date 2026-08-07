// POJ 3171 - Cleaning Shifts
// Model: claude-opus-5
//
// Minimum-cost cover of the second range [M, E] by a subset of the given
// intervals [T1, T2] with salary S: every second M..E must lie in at least one
// chosen interval.
//
// dp[t] = minimum cost to cover every second in M..t, with dp[M-1] = 0 for the
// empty prefix.  An interval [T1, T2] of salary S extends any prefix that
// already reaches somewhere in [T1-1, T2-1] (reaching earlier than T1-1 would
// leave a hole, reaching past T2-1 gains nothing), so
//     dp[T2] <- min(dp[T2], S + min{ dp[j] : T1-1 <= j <= T2-1 }).
// Processing intervals in increasing T2 makes every query read only positions
// strictly below the position it writes, so a single pass suffices.  A segment
// tree over the E-M+2 prefix positions answers each range minimum in
// O(log(E-M)); total O(N log(E-M)).  Answer is dp[E], or -1 if unreachable.
//
// Notes:
//  - The cost bound is N*S <= 10000 * 500000 = 5e9, past 32 bits, so the
//    accumulator is long long and the answer is printed through std::cout
//    (POJ's C++ compiler predates %lld).
//  - The statement guarantees M <= T1 <= T2 <= E.  Intervals are clamped to
//    [M, E] and out-of-range ones dropped anyway: that is the semantically
//    correct reading if the data ever strays outside, which the discuss board
//    argues about.
//  - Input is consumed until EOF, so a multi-dataset file would also work.

#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

const long long INF = 1000000000000000000LL;
const int SZ = 131072; /* >= E-M+2 <= 86401, rounded up to a power of two */

static long long tr[2 * SZ];

static void tmin(int p, long long v) {
    p += SZ;
    if (tr[p] <= v) return;
    tr[p] = v;
    for (p >>= 1; p >= 1; p >>= 1) {
        long long m = tr[2 * p] < tr[2 * p + 1] ? tr[2 * p] : tr[2 * p + 1];
        if (tr[p] == m) break;
        tr[p] = m;
    }
}

static long long tquery(int l, int r) { /* inclusive range minimum */
    long long res = INF;
    if (l > r) return res;
    for (l += SZ, r += SZ + 1; l < r; l >>= 1, r >>= 1) {
        if (l & 1) { if (tr[l] < res) res = tr[l]; ++l; }
        if (r & 1) { --r; if (tr[r] < res) res = tr[r]; }
    }
    return res;
}

struct Job {
    int t1, t2, s;
};

static bool byEnd(const Job &a, const Job &b) {
    return a.t2 < b.t2;
}

static Job job[10005];

int main() {
    int n, M, E;
    while (scanf("%d %d %d", &n, &M, &E) == 3) {
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            int t1, t2, s;
            if (scanf("%d %d %d", &t1, &t2, &s) != 3) { t1 = t2 = s = 0; }
            if (t1 < M) t1 = M;
            if (t2 > E) t2 = E;
            if (t1 > t2) continue; /* contributes nothing to [M, E] */
            job[cnt].t1 = t1;
            job[cnt].t2 = t2;
            job[cnt].s = s;
            ++cnt;
        }
        sort(job, job + cnt, byEnd);

        int L = E - M + 2; /* prefix positions: p corresponds to time M-1+p */
        for (int i = 0; i < 2 * SZ; ++i) tr[i] = INF;
        tmin(0, 0);

        for (int i = 0; i < cnt; ++i) {
            int a = job[i].t1 - M;  /* position of time T1-1 */
            int b = job[i].t2 - M;  /* position of time T2-1 */
            long long best = tquery(a, b);
            if (best >= INF) continue;
            tmin(b + 1, best + (long long)job[i].s);
        }

        long long ans = tquery(L - 1, L - 1);
        if (ans >= INF) cout << -1 << "\n";
        else cout << ans << "\n";
    }
    return 0;
}
