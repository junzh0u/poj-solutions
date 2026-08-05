// POJ 2115 - C Looooops
//
// for (v = A; v != B; v += C) in a k-bit unsigned type: count the iterations,
// or FOREVER. The loop body runs x times where x is the smallest non-negative
// solution of the linear congruence
//
//     C * x == (B - A)   (mod 2^k)
//
// Let m = 2^k, t = (B - A) mod m, and g = gcd(C, m) (a power of two, and equal
// to m when C == 0). The congruence is solvable iff g | t; otherwise the value
// never reaches B and the answer is FOREVER. When it is solvable, divide
// through by g: C/g is odd, hence invertible mod m/g, and the answer is
//
//     x = (t/g) * inv(C/g)   (mod m/g)
//
// taken in [0, m/g). Extended Euclid on (C, m) yields both g and the inverse in
// one pass; extgcd(0, m) gives g = m, x = 0, which makes C == 0 fall out
// correctly (0 iterations when A == B, FOREVER otherwise).
//
// All arithmetic is in unsigned long long: k can be 32, so m = 2^32 and the
// intermediate product (t/g mod m/g) * (inv mod m/g) can reach just under 2^64,
// which unsigned wraparound handles exactly and where signed overflow would be
// undefined. The wrap-around difference B - A is likewise computed unsigned and
// masked with m - 1. Input is read and output written digit by digit to avoid
// depending on %llu in POJ's old libc.
//
// Ambiguity: the statement does not say what happens when A == B, but the C
// semantics it quotes settle it -- the condition is tested before the first
// iteration, so the answer is 0 (sample line 1 confirms this).

#include <cstdio>

typedef unsigned long long ull;
typedef long long ll;

static bool readULL(ull &v) {
    int c = getchar();
    while (c != EOF && (c < '0' || c > '9')) c = getchar();
    if (c == EOF) return false;
    ull x = 0;
    while (c >= '0' && c <= '9') { x = x * 10ULL + (ull)(c - '0'); c = getchar(); }
    v = x;
    return true;
}

static void writeULL(ull v) {
    char buf[24];
    int n = 0;
    if (v == 0) buf[n++] = '0';
    while (v > 0) { buf[n++] = (char)('0' + (int)(v % 10ULL)); v /= 10ULL; }
    while (n > 0) putchar(buf[--n]);
    putchar('\n');
}

/* extended gcd on non-negative values; returns g, sets x,y with a*x+b*y=g */
static ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int main() {
    ull A, B, C, k;
    while (readULL(A) && readULL(B) && readULL(C) && readULL(k)) {
        if (A == 0 && B == 0 && C == 0 && k == 0) break;
        ull m = 1ULL << k;              /* 2^k, k <= 32 */
        ull mask = m - 1ULL;
        ull t = (B - A) & mask;         /* wrap-around difference, well defined */
        ull c = C & mask;

        ll x, y;
        ll g = extgcd((ll)c, (ll)m, x, y);
        ull gg = (ull)g;
        if (t % gg != 0ULL) { printf("FOREVER\n"); continue; }

        ull mm = m / gg;                /* modulus of the reduced congruence */
        ll xr = x % (ll)mm;
        if (xr < 0) xr += (ll)mm;
        ull res = ((ull)xr % mm) * ((t / gg) % mm) % mm;
        writeULL(res);
    }
    return 0;
}
