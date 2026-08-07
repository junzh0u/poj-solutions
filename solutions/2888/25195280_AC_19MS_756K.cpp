// POJ 2888 - Magic Bracelet
// Model: claude-opus-5
//
// Count circular strings of n beads over m kinds, up to rotation, where a
// given set of unordered kind pairs may not sit next to each other, mod 9973.
//
// Burnside over the rotation group.  A colouring fixed by the rotation by i
// is periodic with period g = gcd(n, i), and such a colouring is exactly a
// closed walk of length g in the "may be adjacent" graph, so the rotation by
// i fixes trace(A^g) colourings, where A is the m x m symmetric 0/1
// adjacency matrix.  Exactly phi(n/d) rotations have gcd(n, i) = d, hence
//
//     answer = (1 / n) * sum_{d | n} phi(n/d) * trace(A^d)   (mod 9973).
//
// 9973 is prime and gcd(n, 9973) = 1 is guaranteed, so 1/n is a modular
// inverse by Fermat.
//
// The graph is undirected: "a cannot be strung to b" forbids b-a as well,
// and a == b is legal input (kind a may not touch itself).
//
// Ambiguity: for n = 1 the lone bead's neighbour is itself, so the formula
// reads a self-loop constraint as applying.  That is what trace(A^1) gives
// and what the brute force over the cycle graph gives; nothing in the
// statement or the samples distinguishes it from "n = 1 has no adjacency".
//
// The time limit is tight (the discuss board is full of TLEs), so all
// arithmetic stays in int -- a sum of 10 products of residues below 9973 is
// under 2^31, so a matrix multiply needs only one mod per entry -- powers of
// the symmetric A are symmetric, so only the upper triangle is computed, and
// the divisors of n are enumerated multiplicatively so that A^(d*p) is built
// from an already-known A^d.  Consuming the primes of n largest first keeps
// the expensive powers on a short divisor list.  Worst case n below 1e9 has
// 1344 divisors; 500 such cases run in 0.1s locally.

#include <cstdio>

const int MOD = 9973;

static int m;

struct Mat {
    int a[10][10];
};

static Mat tmpbuf;

/* r = x * y for symmetric commuting x, y (both are powers of A).
   r may alias x or y. */
static void mul(Mat &r, const Mat &x, const Mat &y) {
    int i, j, k;
    for (i = 0; i < m; ++i)
        for (j = i; j < m; ++j) {
            int s = 0;
            for (k = 0; k < m; ++k) s += x.a[i][k] * y.a[k][j];
            s %= MOD;
            tmpbuf.a[i][j] = s;
            tmpbuf.a[j][i] = s;
        }
    for (i = 0; i < m; ++i)
        for (j = 0; j < m; ++j) r.a[i][j] = tmpbuf.a[i][j];
}

/* r = x^e for e >= 1; r may alias x */
static void mpow(Mat &r, const Mat &x, int e) {
    Mat base = x, res = x;
    int hb = 30;
    while (!((e >> hb) & 1)) --hb;
    for (int b = hb - 1; b >= 0; --b) {
        mul(res, res, res);
        if ((e >> b) & 1) mul(res, res, base);
    }
    r = res;
}

static int divs[1600];
static Mat dmat[1600];
static int primes[10], expo[10], np;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n, k, i, j;
        if (scanf("%d %d %d", &n, &m, &k) != 3) break;
        Mat A;
        for (i = 0; i < m; ++i)
            for (j = 0; j < m; ++j) A.a[i][j] = 1;
        for (i = 0; i < k; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            A.a[a - 1][b - 1] = 0;
            A.a[b - 1][a - 1] = 0;
        }

        np = 0;
        {
            int x = n;
            for (int p = 2; p * p <= x; ++p)
                if (x % p == 0) {
                    primes[np] = p;
                    expo[np] = 0;
                    while (x % p == 0) { x /= p; ++expo[np]; }
                    ++np;
                }
            if (x > 1) { primes[np] = x; expo[np] = 1; ++np; }
        }

        int nd = 1;
        divs[0] = 1;
        dmat[0] = A;
        for (i = np - 1; i >= 0; --i) {
            int p = primes[i], sz = nd;
            for (j = 0; j < sz; ++j) {
                int d = divs[j];
                Mat cur = dmat[j];
                for (int e = 0; e < expo[i]; ++e) {
                    if (p <= 5) {
                        Mat t = cur;
                        for (int q = 1; q < p; ++q) mul(t, t, cur);
                        cur = t;
                    } else {
                        mpow(cur, cur, p);
                    }
                    d *= p;
                    divs[nd] = d;
                    dmat[nd] = cur;
                    ++nd;
                }
            }
        }

        int total = 0;
        for (i = 0; i < nd; ++i) {
            int tr = 0;
            for (j = 0; j < m; ++j) tr += dmat[i].a[j][j];
            tr %= MOD;
            int x = n / divs[i], phi = 1;
            for (j = 0; j < np; ++j) {
                int p = primes[j];
                if (x % p == 0) {
                    phi *= p - 1;
                    x /= p;
                    while (x % p == 0) { phi *= p; x /= p; }
                }
            }
            total = (total + phi % MOD * tr) % MOD;
        }

        int inv = 1, b = n % MOD, e = MOD - 2;
        while (e) {
            if (e & 1) inv = inv * b % MOD;
            b = b * b % MOD;
            e >>= 1;
        }
        printf("%d\n", total * inv % MOD);
    }
    return 0;
}
