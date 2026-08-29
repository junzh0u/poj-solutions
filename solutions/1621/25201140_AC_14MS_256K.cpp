// POJ 1621 - Polly Nomials
// Model: claude-sonnet-5
//
// The PDA has no memory beyond the running display, so a term a_i (added at
// the Horner step for x^i) is entered by keying its decimal digits, and
// multiplying the running value by x costs a '*' plus an 'x' press -- except
// the very first multiplication, which is free of the '*' key because the
// leading coefficient is guaranteed to be 1 (1*x is entered by simply
// pressing 'x'). This is Horner's rule (confirmed by the board thread
// discussing this problem, message 75883: "Horner's rule"), and the given
// examples ("x,+,2,*,x,*,x,+,1,1,=" for x^3+2x^2+11) match it exactly even
// though the fused first multiplication can be interleaved with an addition
// before the remaining multiplications.
//
// So for degree n (a_n = 1 always) with coefficients a_{n-1..0}:
//   cost = 2*n + sum over nonzero a_i (i = 0..n-1) of (1 + digit-count(a_i))
// The 2*n accounts for the n multiplications by x (first one 1 press, the
// remaining n-1 at 2 presses each) plus the trailing '=' press.
// Cost does not depend on x's value (only 1 or -1); the value itself is
// simply sum a_i * x^i.

#include <cstdio>

int main() {
    int n;
    int caseNum = 0;
    while (scanf("%d", &n) == 1) {
        if (n == 0) {
            long long xEnd;
            scanf("%lld", &xEnd); // sentinel line "0 0"
            break;
        }
        static long long coef[105];
        for (int i = n; i >= 0; --i) {
            scanf("%lld", &coef[i]);
        }
        long long x;
        scanf("%lld", &x);

        long long value = 0;
        long long xp = 1;
        for (int i = 0; i <= n; ++i) {
            value += coef[i] * xp;
            xp *= x;
        }

        long long cost = 2LL * n;
        for (int i = 0; i < n; ++i) {
            if (coef[i] != 0) {
                long long v = coef[i];
                int d = 0;
                while (v > 0) { ++d; v /= 10; }
                cost += 1 + d;
            }
        }

        ++caseNum;
        printf("Polynomial %d: %lld %lld\n", caseNum, value, cost);
    }
    return 0;
}
