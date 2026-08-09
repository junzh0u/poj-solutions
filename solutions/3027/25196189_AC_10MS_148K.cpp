// POJ 3027 - Base equality
// Model: claude-sonnet-5
// For each test case (B1,B2,r1,r2), find the largest i with r1<i<r2 such
// that the digit sequence of i written in base B1 is, read as a number in
// base B2, an exact multiple of i (i.e. a positive integer c exists with
// value_base_B2(digits) == c*i). Digits stay < B1 < B2 automatically, so
// any digit string of i in base B1 is also a valid digit string in base
// B2. Since B2>B1, that base-B2 value is always >= i (equal only for a
// single digit, giving c=1), so simply brute force i from r2-1 down to
// r1+1 (r2<=10000, so the whole scan is cheap), convert to base B1 digits,
// re-evaluate those digits in base B2, and test divisibility by i; take
// the first (largest) hit. No ambiguity found in the statement or the
// discuss board beyond the well-documented confusion that all numbers in
// the input/output are decimal even though B1/B2 conversions are involved
// (message 125630, 63429): the sample's 9240 is decimal for the digit
// string 6,10,4,0 in base 11, which read in base 14 is divisible by 9240.
#include <cstdio>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        long long b1, b2, r1, r2;
        scanf("%lld %lld %lld %lld", &b1, &b2, &r1, &r2);
        long long ans = -1;
        for (long long i = r2 - 1; i > r1; --i) {
            long long k = i;
            long long value2 = 0;
            long long pw = 1;
            while (k != 0) {
                long long d = k % b1;
                value2 += d * pw;
                pw *= b2;
                k /= b1;
            }
            if (value2 % i == 0) {
                ans = i;
                break;
            }
        }
        if (ans == -1) printf("Non-existent.\n");
        else printf("%lld\n", ans);
    }
    return 0;
}
