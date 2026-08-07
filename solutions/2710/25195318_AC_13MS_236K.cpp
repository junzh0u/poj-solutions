// POJ 2710 - Consecutive Digits
// Model: claude-opus-5
//
// Print base-7 digits at positions b..e (0 = first digit right of the point)
// of the rational n/d, with 0 <= n,d <= 5000 and 0 <= b <= e <= 250.
//
// Plain long division in base 7: drop the integer part with r = n % d, then
// repeatedly r *= 7; digit = r / d; r %= d. Position i is produced by the
// (i+1)-th such step. Since r < d <= 5000, r*7 < 35000 fits in int and no
// big integers or floating point are needed; e <= 250 bounds the loop.
//
// Output format is the one place this problem bites (the discuss board is full
// of Presentation Errors): exactly
//   "Problem set k: n / d, base 7 digits b through e: result"
// with a space after the comma and a space after the final colon, and the
// newline printed after the digits, not before the line.

#include <cstdio>

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    for (int k = 1; k <= t; ++k) {
        int n, d, b, e;
        if (scanf("%d %d %d %d", &n, &d, &b, &e) != 4) break;
        printf("Problem set %d: %d / %d, base 7 digits %d through %d: ",
               k, n, d, b, e);
        if (d == 0) { printf("\n"); continue; }  // statement forbids it; cheap RE insurance
        int r = n % d;
        for (int i = 0; i <= e; ++i) {
            r *= 7;
            if (i >= b) printf("%d", r / d);
            r %= d;
        }
        printf("\n");
    }
    return 0;
}
