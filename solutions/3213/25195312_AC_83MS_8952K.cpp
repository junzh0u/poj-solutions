// POJ 3213 - PM 3
// Model: claude-opus-5
//
// Verify C = A*B (N,P,M <= 1000) knowing at most ONE element of C is wrong.
// A full multiply is O(NPM) = 1e9 and TLEs, so use Freivalds' check with the
// all-ones vector x = (1,...,1) -- which is exact here, not probabilistic:
// D = A*B - C has at most one nonzero entry, so (Dx)_i = D[i][c] can never
// cancel, and the erroneous row r is exactly the row where the row sum of A*B
// disagrees with the row sum of C.  Row sums of A*B come from
//   rowB[k] = sum_j B[k][j],   z[i] = sum_k A[i][k]*rowB[k],
// i.e. O(PM + NP).  With r known, only that one row of A*B is multiplied out
// (O(PM)) to locate the column c and the corrected value v.  Total O(NP+PM+NM).
//
// Ranges: |A|,|B| <= 1000 and |C| <= 2e9, so C fits in int, but a row sum
// reaches 1000*1000*1000 = 1e12 (A*B) / 2e12 (C) -- accumulators are long long.
// The corrected value v is bounded by P*1000*1000 = 1e9 and prints as int, so
// no %lld is needed.  Input is 3e6 integers, so parsing is fread-based (the
// statement warns iostream is too slow).
//
// Ambiguities: none in the rule; the guarantee "at most one incorrect element"
// is what makes the all-ones probe deterministic.  Output is "Yes"/"No" with
// exactly that capitalisation, and r,c are 1-based.

#include <cstdio>

static const int MAXN = 1000;
static int A[MAXN][MAXN], B[MAXN][MAXN], C[MAXN][MAXN];
static long long rowB[MAXN], zr[MAXN], wr[MAXN], acc[MAXN];

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int readInt() {
    int c = gc();
    while (c != -1 && c != '-' && (c < '0' || c > '9')) c = gc();
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    long long x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return (int)(x * sgn);
}

int main() {
    int n = readInt(), p = readInt(), m = readInt();
    int i, j, k;

    for (i = 0; i < n; ++i)
        for (j = 0; j < p; ++j) A[i][j] = readInt();
    for (i = 0; i < p; ++i)
        for (j = 0; j < m; ++j) B[i][j] = readInt();

    for (i = 0; i < p; ++i) {
        long long s = 0;
        const int *row = B[i];
        for (j = 0; j < m; ++j) s += row[j];
        rowB[i] = s;
    }
    for (i = 0; i < n; ++i) {
        long long s = 0;
        const int *row = A[i];
        for (j = 0; j < p; ++j) s += (long long)row[j] * rowB[j];
        zr[i] = s;
    }
    for (i = 0; i < n; ++i) {
        long long s = 0;
        int *row = C[i];
        for (j = 0; j < m; ++j) { int v = readInt(); row[j] = v; s += v; }
        wr[i] = s;
    }

    int r = -1;
    for (i = 0; i < n; ++i)
        if (zr[i] != wr[i]) { r = i; break; }
    if (r < 0) { printf("Yes\n"); return 0; }

    for (j = 0; j < m; ++j) acc[j] = 0;
    for (k = 0; k < p; ++k) {
        long long a = A[r][k];
        if (a == 0) continue;
        const int *row = B[k];
        for (j = 0; j < m; ++j) acc[j] += a * row[j];
    }
    int c = -1;
    for (j = 0; j < m; ++j)
        if (acc[j] != (long long)C[r][j]) { c = j; break; }
    if (c < 0) { printf("Yes\n"); return 0; }

    printf("No\n%d %d\n%d\n", r + 1, c + 1, (int)acc[c]);
    return 0;
}
