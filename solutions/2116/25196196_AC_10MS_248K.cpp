// POJ 2116 - Death to Binary?
// Model: claude-sonnet-5
// Approach: Fibonacci-base (Zeckendorf-style) addition. F0=1, F1=2, F_n=F_{n-1}+F_{n-2}.
// Each input digit string (up to 40 digits, possibly with leading zeros, not
// necessarily canonical) is converted to its true integer value by summing the
// weights of set bits. Since a 40-digit all-ones number is only ~4.3*10^8, and
// the sum of two such numbers is under 10^9, plain 64-bit arithmetic is exact
// with huge headroom (no bignum needed).
//
// The canonical (Zeckendorf) representation of a value is rebuilt from scratch
// with the standard greedy algorithm (take the largest F_i <= remaining value,
// subtract, continue downward): this is guaranteed by Zeckendorf's theorem to
// produce a representation with no two adjacent 1s, and — critically — it does
// NOT touch the original (possibly non-canonical) bit patterns of X or Y at all.
// The discuss board (message 97518) flags that naive left-to-right or
// right-to-left merging of adjacent 1 bits in the original string can cause new
// adjacent 1s to reappear further up (e.g. 10101101), which a single pass of
// local merging mishandles; rebuilding from the numeric value sidesteps that
// class of bug entirely.
//
// Board-confirmed edge cases handled: inputs may have leading zeros; "0 0" is
// legal input and 0's canonical representation is a single digit "0"; a 40+40
// digit sum can need up to 43 digits. Values are never negative.
//
// Output format: width W = 2 + len(canonical(X+Y)) (canonical(X+Y) is always at
// least as long as canonical(X) and canonical(Y) since addition of non-negative
// values is monotonic in Zeckendorf length). Arithmetic uses long long: F[80]
// grown out that far would overflow 32-bit (F index ~45 already exceeds 2^31),
// even though only indices up to ~43 are ever needed for values in range.
// Line 1 is canonical(X) right
// justified in width W; line 2 is "+" then canonical(Y) right justified in the
// remaining W-1 columns; line 3 is two spaces then dashes matching the length of
// canonical(X+Y); line 4 is two spaces then canonical(X+Y). A blank line follows
// every instance (confirmed on the board, message 340634).
//
// Language: submitted under G++ (language 0) per board message 181913 reporting
// C++ WA / G++ AC on this problem; arithmetic fits well within 32-bit range
// regardless, so this is a defensive choice rather than one forced by overflow.

#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

static long long F[80];

string canonical(long long v) {
    if (v == 0) return "0";
    int m = 0;
    while (m + 1 < 80 && F[m + 1] <= v) m++;
    string s(m + 1, '0');
    long long rem = v;
    for (int i = m; i >= 0; i--) {
        if (F[i] <= rem) {
            s[m - i] = '1';
            rem -= F[i];
        }
    }
    return s;
}

long long parseVal(const string &s) {
    long long v = 0;
    int n = (int)s.size();
    for (int j = 0; j < n; j++) {
        if (s[j] == '1') v += F[n - 1 - j];
    }
    return v;
}

int main() {
    F[0] = 1; F[1] = 2;
    for (int i = 2; i < 80; i++) F[i] = F[i - 1] + F[i - 2];

    string sx, sy;
    while (cin >> sx >> sy) {
        long long x = parseVal(sx);
        long long y = parseVal(sy);
        string cx = canonical(x);
        string cy = canonical(y);
        string cs = canonical(x + y);

        int W = 2 + (int)cs.size();
        if ((int)cx.size() > W) W = (int)cx.size();
        if ((int)cy.size() + 1 > W) W = (int)cy.size() + 1;

        // line 1
        for (int i = 0; i < W - (int)cx.size(); i++) putchar(' ');
        cout << cx << "\n";
        // line 2
        putchar('+');
        for (int i = 0; i < W - 1 - (int)cy.size(); i++) putchar(' ');
        cout << cy << "\n";
        // line 3
        putchar(' '); putchar(' ');
        for (size_t i = 0; i < cs.size(); i++) putchar('-');
        putchar('\n');
        // line 4
        putchar(' '); putchar(' ');
        cout << cs << "\n";
        cout << "\n";
    }
    return 0;
}
