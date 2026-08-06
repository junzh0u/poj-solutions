// POJ 2891 - Strange Way to Express Integers
//
// Given k pairs (a_i, r_i), find the smallest non-negative m with
// m == r_i (mod a_i) for every i, or -1 if no such m exists.
//
// The moduli are NOT guaranteed pairwise coprime, so the textbook CRT does not
// apply.  Instead the congruences are merged pairwise into a single one:
// given x == R (mod M) and x == r (mod a), write x = R + M*t; then
// M*t == r - R (mod a), which is solvable iff g = gcd(M, a) divides r - R --
// that divisibility test is the whole "-1" case.  When it holds, dividing
// through by g gives (M/g)*t == (r-R)/g (mod a/g) with M/g invertible mod a/g,
// so t == (r-R)/g * inv(M/g) (mod a/g).  Substituting back yields the merged
// congruence x == R + M*t (mod lcm(M, a)).  Starting from the trivial
// x == 0 (mod 1) and folding in each pair costs O(k log) per test case.
//
// Overflow is the real trap: the answer may need the full 64-bit range and
// __int128 is not available, so the product d * inv taken mod (a/g) is done
// with a binary (Russian peasant) mulmod in unsigned long long -- with the
// modulus below 2^63 every intermediate addition stays under 2^64.  The other
// two products, M*t and M/g*a, are bounded by the final lcm and so fit by the
// statement's own guarantee.  I/O is hand-rolled with getchar/putchar because
// POJ's compilers disagree about the 64-bit printf specifier.
//
// Ambiguity: the statement calls m "non-negative", so when every remainder is
// zero the answer is 0 rather than lcm(a_i) -- the merged residue is already
// the minimal non-negative solution and must not be replaced by the modulus.
// Input runs to EOF with no case count, and a test case whose system is
// inconsistent still has all of its remaining pairs consumed before -1 is
// printed.

#include <cstdio>

typedef long long ll;
typedef unsigned long long ull;

/* (a*b) % m without overflowing 64 bits: binary (Russian peasant) product.
   Requires m < 2^63 so that a+a and r+a stay below 2^64 in unsigned. */
static ll mulmod(ll a, ll b, ll m) {
    if (m <= 1) return 0;
    ull ua = (ull)(a % m), ub = (ull)(b % m), um = (ull)m, r = 0;
    while (ub) {
        if (ub & 1ULL) {
            r += ua;
            if (r >= um) r -= um;
        }
        ua += ua;
        if (ua >= um) ua -= um;
        ub >>= 1;
    }
    return (ll)r;
}

static ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

static ll gcdll(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

/* modular inverse of a mod m, assuming gcd(a,m)==1 */
static ll modinv(ll a, ll m) {
    if (m == 1) return 0;
    ll x, y;
    exgcd(a % m, m, x, y);
    x %= m;
    if (x < 0) x += m;
    return x;
}

/* Read one non-negative integer; returns 0 at EOF.  Done by hand because
   POJ's older compilers disagree about the 64-bit scanf/printf specifier. */
static int readnum(ll &out) {
    int c = getchar();
    while (c != EOF && (c < '0' || c > '9') && c != '-') c = getchar();
    if (c == EOF) return 0;
    int neg = 0;
    if (c == '-') { neg = 1; c = getchar(); }
    ll v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    out = neg ? -v : v;
    return 1;
}

static void putnum(ll v) {
    char buf[24];
    int n = 0;
    if (v == 0) buf[n++] = '0';
    while (v > 0) { buf[n++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (n) putchar(buf[--n]);
    putchar('\n');
}

int main() {
    ll k;
    while (readnum(k)) {
        ll M = 1, R = 0;   /* x == R (mod M); starts as the trivial congruence */
        int bad = 0;
        for (ll i = 0; i < k; ++i) {
            ll a = 1, r = 0;
            if (!readnum(a) || !readnum(r)) { a = 1; r = 0; }
            if (bad) continue;
            if (a <= 0) continue;
            r %= a;
            if (r < 0) r += a;

            ll g = gcdll(M, a);
            ll diff = r - R;
            if (diff % g != 0) { bad = 1; continue; }

            ll m2 = a / g;               /* new factor of the lcm */
            ll d = (diff / g) % m2;      /* may be negative */
            if (d < 0) d += m2;
            ll inv = modinv((M / g) % m2, m2);
            ll t = mulmod(d, inv, m2);   /* x = R + M*t */

            R = R + M * t;
            M = M / g * a;               /* lcm(M, a) */
        }
        if (bad) fputs("-1\n", stdout);
        else putnum(R);
    }
    return 0;
}
