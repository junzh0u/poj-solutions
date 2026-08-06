// POJ 2718 - Smallest Difference
//
// Split a set of distinct digits into two non-empty groups, order each group
// freely to form an integer, and minimise the absolute difference.
//
// Two observations make brute force cheap enough:
//
//   1. The split is always as even as possible.  If the parts have lengths
//      a <= b with b - a >= 2, then the larger part is at least 10^(b-1) while
//      the smaller is at most 10^a - 1 <= 10^(b-2) - 1, so the difference
//      exceeds 9*10^(b-2); an evenly split arrangement always beats that.  So
//      only the split at n/2 needs to be considered, and permuting all n digits
//      covers every assignment of digits to the two sides.
//   2. The answer depends only on the digit set, and there are just 2^10 of
//      them, so memoising on the bitmask bounds the total work no matter how
//      many test cases arrive.  Summed over every possible input the whole
//      program does under 10M permutations.
//
// Leading zeros are the trap: a part may start with 0 only when that part is
// the single digit 0 (the statement's "unless the resulting integer is 0").
// Since the digits are distinct at most one part can be a bare 0, and the
// checks below reject a 0 in front of any part longer than one digit -- on both
// sides of the split, not just the first.
//
// Values stay small: with the even split no part exceeds 5 digits, so int is
// plenty and long long (a C++11 extension here) is not needed.

#include <cstdio>
#include <algorithm>
using namespace std;

static int memo[1024];
static bool done_[1024];

static int solve(int mask) {
    if (done_[mask]) return memo[mask];
    int d[10], n = 0;
    for (int i = 0; i < 10; ++i) if ((mask >> i) & 1) d[n++] = i;
    int a = n / 2;              /* length of the first part */
    int best = -1;
    do {
        if (a > 1 && d[0] == 0) continue;
        if (n - a > 1 && d[a] == 0) continue;
        int x = 0, y = 0;
        for (int i = 0; i < a; ++i) x = x * 10 + d[i];
        for (int i = a; i < n; ++i) y = y * 10 + d[i];
        int diff = x > y ? x - y : y - x;
        if (best < 0 || diff < best) best = diff;
    } while (next_permutation(d, d + n));
    done_[mask] = true;
    memo[mask] = best;
    return best;
}

/* one case per line; blank lines and stray \r are skipped */
static int readCase() {
    int mask = 0, c, got = 0;
    while ((c = getchar()) != EOF) {
        if (c >= '0' && c <= '9') { mask |= 1 << (c - '0'); got = 1; }
        else if (c == '\n' && got) return mask;
    }
    return mask;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 0; tc < T; ++tc) {
        int mask = readCase();
        if (!mask) break;
        printf("%d\n", solve(mask));
    }
    return 0;
}
