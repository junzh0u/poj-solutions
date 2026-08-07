// POJ 2065 - SETI
// Model: claude-opus-5
//
// Invert the transcription f(k) = sum_{i=0..n-1} a_i * k^i (mod p) for
// k = 1..n, given the n transcribed values ('*' = 0, 'a'..'z' = 1..26).
// That is an n x n Vandermonde system over GF(p) with nodes 1..n; the
// statement guarantees p prime with p > n and p > 26, so the nodes are
// distinct and nonzero mod p and the system has exactly one solution
// (the discuss board confirms the judge is not a special judge, which
// is consistent with the answer being unique).  Gauss-Jordan forward
// elimination with modular inverses, then back substitution.
// n <= 70 and p <= 30000, so O(n^3) per case is trivial.
//
// Arithmetic: every operand is kept reduced into [0, p), so each product
// is below 30000^2 = 9.0e8 < 2^31 and plain int suffices.  The board has
// two reports of "int WA, long long AC"; the codes posted there use the
// LCM elimination variant, which accumulates unreduced sums of ~n such
// products and does overflow.  Reducing after every multiply removes the
// problem without needing a 64-bit type.
//
// Output: values separated by single spaces, one line per case, in
// ascending order of i.  a_i can exceed 26 (see the third sample line).

#include <cstdio>
#include <cstring>

static const int MAXN = 80;

static int mat[MAXN][MAXN + 1];
static int ans[MAXN];
static char buf[256];

/* modular inverse of a (0 < a < p) with p prime, via extended Euclid */
static int modinv(int a, int p) {
    int r0 = p, r1 = a;
    int t0 = 0, t1 = 1;
    while (r1 != 0) {
        int q = r0 / r1;
        int r2 = r0 - q * r1;
        int t2 = t0 - q * t1;
        r0 = r1; r1 = r2;
        t0 = t1; t1 = t2;
    }
    t0 %= p;
    if (t0 < 0) t0 += p;
    return t0;
}

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases-- > 0) {
        int p;
        if (scanf("%d %255s", &p, buf) != 2) break;
        int n = (int)strlen(buf);

        int i, j, k;
        for (i = 0; i < n; ++i) {
            int kk = (i + 1) % p;      /* evaluation point k = i+1 */
            int pw = 1 % p;
            for (j = 0; j < n; ++j) {
                mat[i][j] = pw;
                pw = (pw * kk) % p;
            }
            mat[i][n] = (buf[i] == '*') ? 0 : (buf[i] - 'a' + 1);
        }

        /* Gauss-Jordan style forward elimination mod p */
        for (j = 0; j < n; ++j) {
            int piv = -1;
            for (i = j; i < n; ++i) {
                if (mat[i][j] != 0) { piv = i; break; }
            }
            if (piv < 0) continue;      /* cannot happen: Vandermonde is regular */
            if (piv != j) {
                for (k = j; k <= n; ++k) {
                    int t = mat[j][k]; mat[j][k] = mat[piv][k]; mat[piv][k] = t;
                }
            }
            int inv = modinv(mat[j][j], p);
            for (k = j; k <= n; ++k) mat[j][k] = (mat[j][k] * inv) % p;
            for (i = j + 1; i < n; ++i) {
                int f = mat[i][j];
                if (f == 0) continue;
                for (k = j; k <= n; ++k) {
                    /* both operands are already reduced into [0, p), and
                       p <= 30000 so the product stays below 9.0e8 < 2^31 */
                    int v = mat[i][k] - (mat[j][k] * f) % p;
                    mat[i][k] = (v < 0) ? v + p : v;
                }
            }
        }

        /* back substitution */
        for (i = n - 1; i >= 0; --i) {
            int acc = mat[i][n];
            for (j = i + 1; j < n; ++j) {
                int sub = (mat[i][j] * ans[j]) % p;
                acc -= sub;
                if (acc < 0) acc += p;
            }
            ans[i] = acc % p;
        }

        for (i = 0; i < n; ++i) {
            if (i) putchar(' ');
            printf("%d", ans[i]);
        }
        putchar('\n');
    }
    return 0;
}
