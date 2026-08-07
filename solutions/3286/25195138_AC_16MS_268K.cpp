// POJ 3286 - How many 0's?
// Model: claude-opus-5
//
// Count the '0' digits written when every natural number in [m, n] is written
// out in decimal, m <= n both unsigned 32-bit, input terminated by a line with
// m negative.
//
// Approach: a per-position digit count. For a place value p (p = 1, 10, ...)
// split n as high = n/(10p), cur = (n/p)%10, low = n%p. The numbers in 1..n
// whose digit at place p is 0 are counted by
//     cur == 0 : (high-1)*p + low + 1
//     cur >  0 : high*p
// The "-1" and the exclusion of p once 10p > n are what forbid leading zeros:
// a zero only counts when some nonzero digit sits above it. Summing over all
// p with 10p <= n gives Z(n) = zeros written for 1..n; the answer for [m, n]
// is Z(n) - Z(m-1). O(log n) per query.
//
// Ambiguity: the range starts at 0, and 0 is itself written "0", contributing
// one zero that no digit-position formula produces (it has no nonzero digit
// above it). The sample settles it: "0 500" is 92 while 1..500 only accounts
// for 91. So the prefix used is Z(n) + 1 over 0..n, and m == 0 is handled
// without forming m-1.
//
// The result exceeds 32 bits (0..4294967295 gives 3825876150), so all
// arithmetic is 64-bit; iostream is used rather than scanf/printf because the
// 64-bit format specifier differs between POJ's MSVC and GCC front ends.

#include <iostream>

typedef long long ll;

/* zeros written in the decimal representations of 1..n (no leading zeros) */
static ll zerosUpTo(ll n) {
    if (n <= 0) return 0;
    ll cnt = 0;
    for (ll p = 1; p <= n / 10; p *= 10) {
        ll high = n / (p * 10);
        ll cur = (n / p) % 10;
        ll low = n % p;
        if (cur == 0) cnt += (high - 1) * p + low + 1;
        else cnt += high * p;
    }
    return cnt;
}

/* zeros written in 0..n; the number 0 itself is written "0" */
static ll zerosFrom0(ll n) { return zerosUpTo(n) + 1; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    ll m, n;
    while (std::cin >> m >> n) {
        if (m < 0) break;
        ll ans = zerosFrom0(n) - (m == 0 ? 0 : zerosFrom0(m - 1));
        std::cout << ans << "\n";
    }
    return 0;
}
