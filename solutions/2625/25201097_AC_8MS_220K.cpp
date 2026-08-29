// POJ 2625 - Coupons
// Model: claude-sonnet-5
// Approach: expected number of boxes to collect a complete set of n coupons
// (coupon collector's problem) is E[n] = n * H_n where H_n = sum_{k=1}^n 1/k.
// Compute H_n as an exact reduced fraction using 64-bit integers (n<=22 keeps
// every intermediate value, even before reduction, comfortably inside a
// 64-bit range: den(H_n) divides lcm(1..22)=232792560, and the transient
// numerator during summation never exceeds roughly 2e10). Multiply by n and
// reduce again to get E[n] = P/Q in lowest terms; since H_n's own num/den are
// already coprime, remainder = P mod Q is automatically coprime to Q too
// (gcd(a - k*b, b) = gcd(a,b)), so no further reduction is needed for the
// fractional part that gets printed.
// Ambiguity: the statement's own sample shows a trailing space after an
// integer-only answer ("3 ") while the spec text explicitly forbids trailing
// spaces in any output line; the spec text is honored (no trailing space is
// emitted) since POJ's line comparison is expected to tolerate the
// discrepancy either way.
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

typedef long long ll;

ll gcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        ll num = 0, den = 1; // running H_n fraction
        for (int k = 1; k <= n; ++k) {
            // add 1/k to num/den
            ll g = gcd(den, (ll)k);
            ll newDen = den / g * k;
            ll newNum = num * (newDen / den) + (newDen / k);
            ll g2 = gcd(newNum, newDen);
            if (g2 == 0) g2 = 1;
            num = newNum / g2;
            den = newDen / g2;
        }
        // E = n * H_n = (num*n) / den, reduce
        ll pnum = num * (ll)n;
        ll pden = den;
        ll g3 = gcd(pnum, pden);
        if (g3 == 0) g3 = 1;
        pnum /= g3;
        pden /= g3;

        ll intPart = pnum / pden;
        ll rem = pnum % pden;

        if (rem == 0) {
            printf("%lld\n", intPart);
        } else {
            char intBuf[32], numBuf[32], denBuf[32];
            sprintf(intBuf, "%lld", intPart);
            sprintf(numBuf, "%lld", rem);
            sprintf(denBuf, "%lld", pden);
            size_t intLen = strlen(intBuf);
            size_t numLen = strlen(numBuf);
            size_t denLen = strlen(denBuf);
            size_t dashLen = max(numLen, denLen);
            string prefix(intLen + 1, ' ');
            printf("%s%s\n", prefix.c_str(), numBuf);
            printf("%s ", intBuf);
            for (size_t i = 0; i < dashLen; ++i) putchar('-');
            putchar('\n');
            printf("%s%s\n", prefix.c_str(), denBuf);
        }
    }
    return 0;
}
