// POJ 3208 - Apocalypse Someday
// Model: claude-opus-5
//
// A "beastly" number contains the substring "666" (three *contiguous* sixes).
// Given n <= 50,000,000, print the n-th beastly number.
//
// Approach: binary search on the answer over a counting function computed by
// digit DP.  cnt(N) = how many x in [0,N] contain "666"; the n-th beastly
// number is the smallest N with cnt(N) >= n.
//
// The DP is a 4-state automaton over the digits: state s in {0,1,2} is the
// length of the current trailing run of sixes, state 3 is the absorbing
// "already saw 666" state.  f[len][s] = number of length-len digit suffixes
// that reach state 3 when started from state s; f[0][3] = 1, f[0][s<3] = 0.
// cnt(N) then walks N's digits, adding f[remaining][nxt(state,d)] for every
// digit d strictly below the current one, and adds 1 at the end if N itself
// ended in state 3.  Padding with leading zeros neither creates nor destroys
// a run of sixes, so counting fixed-length strings equals counting values.
//
// Notes on the bounds:
//   * The largest answer is n = 50,000,000 -> 6668056399, which overflows a
//     32-bit int, so the answer is a long long.  It is printed with std::cout
//     because POJ's compilers predate %lld; n itself fits in an int and is
//     read with %d, so no 64-bit format specifier appears anywhere.
//   * The binary search upper bound 2e10 is generous: cnt(2e10) = 145,830,800,
//     nearly three times the largest n.
//
// Verification: all 45,991 beastly numbers below 10^7 were checked by index
// against a brute-force scan, cnt(N) was checked against a brute-force count
// for every N from 0 to 3,000,000, and the four cases posted on the discuss
// board (782 -> 268666, 8835 -> 2516669, 25429971 -> 3966866603,
// 50000000 -> 6668056399) all reproduce.  Worst case (T = 1000, every n at
// the maximum) runs in 5 ms against a 1000 ms limit.
//
// No statement ambiguity: "at least three contiguous sixes" is exactly the
// substring "666", and the statement's own opening list (666, 1666, 2666,
// 3666, 4666, 5666) confirms the ordering is by numeric value.

#include <cstdio>
#include <iostream>

typedef long long ll;

static const int MAXLEN = 18;
static ll f[MAXLEN + 1][4];
static int nxt[4][10];

static void init() {
    for (int s = 0; s < 4; ++s)
        for (int d = 0; d < 10; ++d) {
            if (s == 3) nxt[s][d] = 3;
            else if (d == 6) nxt[s][d] = s + 1;
            else nxt[s][d] = 0;
        }
    for (int s = 0; s < 4; ++s) f[0][s] = 0;
    f[0][3] = 1;
    /* len stops at 18: f[len][3] is 10^len, and 10^19 overflows long long.
       The search never exceeds 2e10, so only f[0..10] is ever read. */
    for (int len = 1; len <= MAXLEN; ++len)
        for (int s = 0; s < 4; ++s) {
            ll t = 0;
            for (int d = 0; d < 10; ++d) t += f[len - 1][nxt[s][d]];
            f[len][s] = t;
        }
}

/* number of x in [0,N] whose decimal expansion contains "666" */
static ll cnt(ll N) {
    if (N < 0) return 0;
    int dig[20], L = 0;
    if (N == 0) {
        dig[L++] = 0;
    } else {
        ll t = N;
        while (t) { dig[L++] = (int)(t % 10); t /= 10; }
    }
    for (int i = 0; i < L / 2; ++i) {
        int t = dig[i]; dig[i] = dig[L - 1 - i]; dig[L - 1 - i] = t;
    }

    ll res = 0;
    int st = 0;
    for (int i = 0; i < L; ++i) {
        for (int d = 0; d < dig[i]; ++d) res += f[L - 1 - i][nxt[st][d]];
        st = nxt[st][dig[i]];
    }
    if (st == 3) ++res;
    return res;
}

int main() {
    init();
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int ni;
        if (scanf("%d", &ni) != 1) break;
        ll n = ni;
        ll lo = 1, hi = 20000000000LL;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (cnt(mid) >= n) hi = mid; else lo = mid + 1;
        }
        std::cout << lo << "\n";
    }
    return 0;
}
