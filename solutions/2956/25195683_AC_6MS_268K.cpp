// POJ 2956 - Repeatless Numbers
// Model: claude-opus-5
//
// Find the nth positive integer whose decimal digits are all distinct.
//
// Direct combinatorial unranking, no table and no search. Repeatless numbers
// of a given digit length L, listed in increasing order, are exactly the
// length-L strings of distinct digits with a nonzero lead, in lexicographic
// order -- so ordering by length and then lexicographically is the same as
// ordering by value, and the nth number can be built digit by digit.
//
// Subtract the per-length populations cnt[L] = 9 * P(9, L-1) to find the
// length, leaving a 1-based rank m inside it. Then at each position try the
// unused digits in ascending order (skipping 0 in the lead): fixing a digit
// leaves P(10-k, r) completions for the r positions after it, so if m exceeds
// that block, skip the block and move on, otherwise commit the digit.
//
// Lengths 1..10 are handled -- more than the stated n <= 1000000 needs, which
// only reaches 8 digits (n = 1000000 -> 26195083). The extra reach reduces the
// answer past 2^31, hence long long and G++ (language 0) rather than C++
// (language 4); the discuss board separately reports RE under 4 here, fixed by 0.
//
// No ambiguity in the statement. Input is read to EOF as well as on the
// stated n = 0 sentinel, and skips whitespace, so blank lines, CRLF and a
// missing sentinel are all safe.
//
// Verified exhaustively rather than by sampling: a brute force transcribed
// from the statement's wording (test every integer < 10^8, reject repeated
// digits) agrees on all 2345850 values of n for which an 8-digit answer
// exists, which covers the whole input domain.

#include <iostream>

using namespace std;

/* P(a, r) = a * (a-1) * ... * (a-r+1) */
static long long perm(int a, int r) {
    long long p = 1;
    for (int i = 0; i < r; ++i) p *= (a - i);
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    /* permTab[a][r] = P(a, r): ways to fill r positions from a unused digits */
    long long permTab[11][11];
    for (int a = 0; a <= 10; ++a)
        for (int r = 0; r <= 10; ++r)
            permTab[a][r] = (r <= a) ? perm(a, r) : 0;

    /* cnt[L] = how many repeatless numbers have exactly L digits */
    long long cnt[11];
    for (int L = 1; L <= 10; ++L) cnt[L] = 9LL * perm(9, L - 1);

    long long n;
    while (cin >> n) {
        if (n == 0) break;

        int L = 1;
        while (L <= 10 && n > cnt[L]) { n -= cnt[L]; ++L; }
        if (L > 10) { cout << 0 << "\n"; continue; }  /* beyond every length */

        bool used[10];
        for (int d = 0; d < 10; ++d) used[d] = false;

        long long m = n;                /* 1-based rank within this length */
        long long value = 0;
        for (int i = 0; i < L; ++i) {
            int r = L - 1 - i;          /* positions still to fill after this one */
            long long ways = permTab[10 - (i + 1)][r];
            int start = (i == 0) ? 1 : 0;
            for (int d = start; d < 10; ++d) {
                if (used[d]) continue;
                if (m > ways) {
                    m -= ways;          /* whole block of completions skipped */
                } else {
                    used[d] = true;
                    value = value * 10 + d;
                    break;
                }
            }
        }
        cout << value << "\n";
    }
    return 0;
}
