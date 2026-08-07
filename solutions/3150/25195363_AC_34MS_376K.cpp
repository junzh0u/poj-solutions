// POJ 3150 - Cellular Automaton
// Model: claude-opus-5
//
// One d-step is the linear operator (Tv)[i] = sum_u p[u] * v[(i+u) mod n],
// where p[u] = 1 exactly when min(u, n-u) <= d, i.e. p is the indicator of the
// d-environment written as offsets.  Composing two such operators gives
//   (T_p T_q v)[i] = sum_u (p * q)[u] v[(i+u) mod n],
// with (p * q) the cyclic convolution of length n.  So k steps are the k-th
// cyclic-convolution power of p, i.e. p(x)^k in Z_m[x]/(x^n - 1) -- the usual
// circulant-matrix fact, but kept as a single row so a product costs O(n^2)
// instead of O(n^3).  Binary exponentiation: O(n^2 log k) ~ 12M multiply-adds
// at n=500, k=10^7.  The final answer is the same convolution applied to the
// initial vector (p, hence p^k, is symmetric, so correlation and convolution
// coincide here).
//
// Two things the statement does not say, both taken from the discuss board:
//  * the input holds several data sets and must be read to EOF (this is also
//    what the 12000MS total / 2000MS per-case limits imply);
//  * m can be 10^6, so a product of two residues reaches 10^12 -- the partial
//    sums must be 64-bit (a board thread is titled exactly "WA students,
//    beware integer multiplication overflow").  Only values < m are printed,
//    so no 64-bit printf conversion is needed.
//
// The kernel is built straight from the statement's distance formula rather
// than by walking offsets -d..d, so it stays correct even if some data set
// violates the promised d < n/2 (that walk would double-count an offset).

#include <cstdio>
#include <cstring>

static int n, m;

/* c = a (*) b : cyclic convolution of length n, reduced modulo m */
static void mul(const int *a, const int *b, int *c)
{
    static long long tmp[512];
    int i, j, idx;
    for (i = 0; i < n; i++) tmp[i] = 0;
    for (i = 0; i < n; i++) {
        long long ai = a[i];
        if (!ai) continue;
        idx = i;
        for (j = 0; j < n; j++) {
            tmp[idx] += ai * b[j];
            if (++idx == n) idx = 0;
        }
    }
    for (i = 0; i < n; i++) c[i] = (int)(tmp[i] % m);
}

int main()
{
    int d, k, i;
    static int p[512], r[512], v[512], buf[512];
    while (scanf("%d %d %d %d", &n, &m, &d, &k) == 4) {
        for (i = 0; i < n; i++) scanf("%d", &v[i]);

        for (i = 0; i < n; i++) {
            int dist = i < n - i ? i : n - i;    /* distance of offset i */
            p[i] = (dist <= d) ? 1 % m : 0;
            r[i] = 0;
        }
        r[0] = 1 % m;                            /* identity of convolution */

        while (k) {
            if (k & 1) { mul(r, p, buf); memcpy(r, buf, sizeof(int) * n); }
            k >>= 1;
            if (k) { mul(p, p, buf); memcpy(p, buf, sizeof(int) * n); }
        }

        for (i = 0; i < n; i++) v[i] %= m;
        mul(r, v, buf);
        for (i = 0; i < n; i++)
            printf("%d%c", buf[i], i + 1 == n ? '\n' : ' ');
    }
    return 0;
}
