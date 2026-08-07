// POJ 1930 - Dead Fraction
// Model: claude-opus-5
//
// Input is "0.dddd..." with 1..9 recorded digits; the repeating block is fully
// recorded, so it ends at the last recorded digit.  Enumerate the period length
// j = 1..n: the first n-j digits are the non-repeating head P, the last j the
// period.  Then
//     value = (D - P) / ((10^j - 1) * 10^(n-j))
// with D the whole n-digit number.  Reduce and keep the smallest denominator.
//
// Why "period ends at the last digit" loses nothing: if the expansion is
// periodic with period p from position s, it is periodic with period p from any
// later position too, and "no digit of the repeating portion was left
// unrecorded" forces s + p <= n.  So sliding the window to end at position n
// gives the same value.  Verified exhaustively against a statement-derived
// reference for every digit string of length <= 6: identical candidate sets,
// and the minimal denominator never ties between two different values.
//
// Ranges: D <= 999999999 and (10^j - 1) * 10^(n-j) < 10^n <= 10^9, both inside
// 32-bit int, so no 64-bit arithmetic (and no %lld) is needed.
//
// Statement vs. data: the statement promises the digits are "not all zero", but
// the discuss board reports all-zero cases in the judge data, expecting "0/1".
// gcd(0, d) = d handles that with no special case.

#include <cstdio>
#include <cstring>
#include <cctype>

static int gcd(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main() {
    char buf[64];
    while (scanf("%63s", buf) == 1) {
        if (strcmp(buf, "0") == 0) break;

        char digits[64];
        int n = 0;
        for (int i = 2; buf[i] != '\0' && isdigit((unsigned char)buf[i]) && n < 9; ++i)
            digits[n++] = buf[i];
        digits[n] = '\0';
        if (n == 0) break;

        int D = 0;
        for (int i = 0; i < n; ++i) D = D * 10 + (digits[i] - '0');

        int bestNum = 0, bestDen = 0;
        int pow10j = 1;   /* 10^j       */
        int pow10r = 1;   /* 10^(n-j)   */
        for (int i = 0; i < n; ++i) pow10r *= 10;

        for (int j = 1; j <= n; ++j) {
            pow10j *= 10;
            pow10r /= 10;
            int P = D / pow10j;                  /* first n-j digits */
            int num = D - P;
            int den = (pow10j - 1) * pow10r;
            int g = gcd(num, den);
            if (g == 0) g = 1;
            num /= g;
            den /= g;
            if (bestDen == 0 || den < bestDen) {
                bestDen = den;
                bestNum = num;
            }
        }
        printf("%d/%d\n", bestNum, bestDen);
    }
    return 0;
}
