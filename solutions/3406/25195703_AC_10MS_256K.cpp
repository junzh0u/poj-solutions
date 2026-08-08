// POJ 3406 - Last digit
// Model: claude-opus-5
//
// Last nonzero digit of C(n,m), n,m <= 1000000.
//
// Write C(n,m) = 2^e2 * 5^e5 * U with U coprime to 10; e2 and e5 come from
// Legendre's formula on the three factorials.  Trailing zeros number exactly
// min(e2,e5), so the answer is (C / 10^min) mod 10, i.e. U * 2^(e2-e5) mod 10
// when e2 >= e5, and plain 5 when e5 > e2 (odd unit times a positive power
// of 5 always ends in 5).
//
// U is odd, so U mod 10 is pinned by U mod 5 alone (r -> r if odd, else r+5).
// U mod 5 = u(n) * u(m)^-1 * u(n-m)^-1, where u(k) = (k! with all 2s and 5s
// removed) mod 5.  Getting u(k) needs only P(k) = (k!/5^v5(k!)) mod 5, which
// obeys P(k) = (-1)^(k/5) * (k%5)! * P(k/5) since every complete block of five
// consecutive non-multiples of 5 has product 4! = -1 mod 5; then divide out
// 2^v2(k!) with an inverse mod 5.  Everything is O(log n) per query, all in
// int, no big integers and no floating point.
//
// Input framing: the statement describes a single line, but an accepted
// solution on the discuss board reads to EOF, so this reads pairs until EOF
// and prints one digit per pair.  A lone case is unaffected.

#include <cstdio>

/* v_p(k!) */
static int legendre(int k, int p) {
    int s = 0;
    while (k) { k /= p; s += k; }
    return s;
}

/* (k! with all factors of 5 removed) mod 5 */
static int P5(int k) {
    static const int fact[5] = {1, 1, 2, 1, 4}; /* i! mod 5 for i=0..4 */
    int r = 1;
    while (k > 0) {
        int q = k / 5;
        if (q & 1) r = (r * 4) % 5;   /* each full block of 5 contributes 4! = -1 */
        r = (r * fact[k % 5]) % 5;
        k = q;
    }
    return r;
}

static int inv5(int a) {          /* inverse mod 5 of a in {1,2,3,4} */
    static const int inv[5] = {0, 1, 3, 2, 4};
    return inv[a % 5];
}

static int powmod(int b, int e, int m) {
    int r = 1 % m;
    b %= m;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

/* (k! with all 2s and 5s removed) mod 5 */
static int unit5(int k) {
    int p = P5(k);
    int t = powmod(2, legendre(k, 2) % 4, 5); /* 2^4 = 1 mod 5 */
    return p * inv5(t) % 5;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int e2 = legendre(n, 2) - legendre(m, 2) - legendre(n - m, 2);
        int e5 = legendre(n, 5) - legendre(m, 5) - legendre(n - m, 5);

        int u = unit5(n) * inv5(unit5(m)) % 5 * inv5(unit5(n - m)) % 5;
        /* lift u (mod 5) to mod 10: the cofactor is odd */
        int u10 = (u % 2 == 1) ? u : u + 5;

        int ans;
        if (e5 > e2) {
            ans = 5;                      /* odd unit times a positive power of 5 */
        } else {
            ans = u10 * powmod(2, e2 - e5, 10) % 10;
        }
        printf("%d\n", ans);
    }
    return 0;
}
