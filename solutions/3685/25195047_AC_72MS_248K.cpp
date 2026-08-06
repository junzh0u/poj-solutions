// POJ 3685 - Matrix
// Model: claude-sonnet-5
// Approach: binary search the answer X; count matrix entries <= X in
// O(N) per check. A(i,j) = j^2 + (i-100000)*j + (i^2+100000*i) is a convex
// quadratic in j with real vertex at j0 = (100000-i)/2, always >= 25000 for
// 1<=i<=50000. Per row, solve j^2 - nb*j + (c-X) <= 0 (nb = 100000-i) via its
// discriminant D and an *integer* sqrt (isqrt, with an explicit correction
// loop against the true D — no floating-point compare is trusted on its
// own), giving candidate jmin/jmax that are nudged by at most one step to
// their exact values. This avoids the "precision" trap the discuss board
// warns cost people over an hour: never compare X against a floating-point
// approximation of A(i,j) itself, only use float sqrt as a starting guess
// that integer arithmetic then verifies and corrects.
// All arithmetic is long long: 100000*i overflows 32-bit int for i up to
// 50000 (board: "integer constants need LL"), and M can reach N*N = 2.5e9,
// which does not fit a 32-bit int either (board: watch N/M's range).
// Ambiguity: with duplicate values, the board confirms each occurrence is
// its own rank (e.g. N=50000, M=1 and M=2 both report -2499849999), which
// is exactly what "smallest X with count(<=X) >= M" computes.
#include <cstdio>
#include <cmath>
typedef long long ll;
static ll N;

static inline ll f(ll i, ll j) {
    return i * i + 100000LL * i + j * j - 100000LL * j + i * j;
}

static ll isqrt_ll(ll n) {
    if (n <= 0) return 0;
    ll s = (ll)sqrtl((long double)n);
    while (s > 0 && s * s > n) s--;
    while ((s + 1) * (s + 1) <= n) s++;
    return s;
}

static ll countLE(ll X) {
    ll total = 0;
    for (ll i = 1; i <= N; ++i) {
        ll nb = 100000 - i; // = -(coeff of j), always positive for i<=50000
        ll c = i * i + 100000LL * i;
        ll D = nb * nb - 4 * (c - X);
        if (D < 0) continue; // row's minimum already exceeds X
        ll s = isqrt_ll(D);
        ll jmax = (nb + s) / 2;
        while (jmax + 1 <= N && f(i, jmax + 1) <= X) jmax++;
        while (jmax >= 1 && f(i, jmax) > X) jmax--;
        ll jmin = (nb - s + 1) / 2;
        while (jmin - 1 >= 1 && f(i, jmin - 1) <= X) jmin--;
        while (jmin <= N && f(i, jmin) > X) jmin++;
        ll lo = jmin < 1 ? 1 : jmin;
        ll hi = jmax > N ? N : jmax;
        if (hi >= lo) total += (hi - lo + 1);
    }
    return total;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        ll M;
        scanf("%lld %lld", &N, &M);
        ll lo = -3000000000LL, hi = 8000000000LL;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (countLE(mid) >= M) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
