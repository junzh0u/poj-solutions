// POJ 2118 - Firepersons
// Model: claude-sonnet-5
// Approach: linear recurrence a_n = sum_{1<=i<=k} a_{n-i} b_i mod 10000 (n>=k),
// a_0..a_{k-1} given directly. For query index i < k, answer is a_i directly.
// For i >= k, build the k x k companion-style transition matrix M
// (row 0 = b_1..b_k, rows 1..k-1 = shifted identity) acting on the state
// vector S = (a_{n+k-1}, ..., a_n). Compute M^p * S0 with p = i-k+1 using
// binary exponentiation where matrix-matrix multiplies are only used to
// square the base matrix, while the accumulator is a vector multiplied by
// the current matrix power only when the corresponding bit of p is set
// (matrix-vector multiply is O(k^2) instead of O(k^3)), which roughly
// halves the work versus full matrix exponentiation then applying to the
// vector at the end. Fixed-size static arrays (k<=100) avoid per-call
// heap allocation across the many instances an input file can hold.
// All input/output values (k<=100, a_i,b_i<10000, i<1e9) fit in 32-bit
// int, so plain int is used for every read/print; only the internal
// matrix-multiply accumulator needs a 64-bit type, avoiding any reliance
// on %lld/%I64d formatting quirks of POJ's older compilers.
// Ambiguity: none noted; sample input/output matches a straightforward
// Fibonacci-mod-10000 reading (k=2,a0=0,a1=1,b1=b2=1,i=6 -> 8).
#include <cstdio>
using namespace std;

static const int MAXK = 100;
static const int MOD = 10000;

int K;
typedef int Mat[MAXK][MAXK];
typedef int Vec[MAXK];

void matmul(const Mat A, const Mat B, Mat C) {
    static long long acc[MAXK];
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) acc[j] = 0;
        for (int t = 0; t < K; ++t) {
            int a = A[i][t];
            if (a == 0) continue;
            const int *Bt = B[t];
            for (int j = 0; j < K; ++j) {
                acc[j] += (long long)a * Bt[j];
            }
        }
        for (int j = 0; j < K; ++j) C[i][j] = (int)(acc[j] % MOD);
    }
}

void matvec(const Mat A, const Vec v, Vec r) {
    for (int i = 0; i < K; ++i) {
        long long acc = 0;
        const int *Ai = A[i];
        for (int t = 0; t < K; ++t) {
            acc += (long long)Ai[t] * v[t];
        }
        r[i] = (int)(acc % MOD);
    }
}

int a[MAXK], b[MAXK];
Mat M, base, tmpMat;
Vec S0, result, tmpVec;

int main() {
    while (true) {
        int k;
        if (scanf("%d", &k) != 1) break;
        K = k;
        if (K == 0) break;
        for (int idx = 0; idx < K; ++idx) scanf("%d", &a[idx]);
        for (int idx = 0; idx < K; ++idx) scanf("%d", &b[idx]);
        int i;
        scanf("%d", &i);

        if (i < K) {
            printf("%d\n", a[i]);
            continue;
        }

        int p = i - K + 1; // >=1

        for (int r = 0; r < K; ++r)
            for (int c = 0; c < K; ++c)
                M[r][c] = 0;
        for (int j = 0; j < K; ++j) M[0][j] = b[j] % MOD;
        for (int r = 1; r < K; ++r) M[r][r-1] = 1;

        for (int j = 0; j < K; ++j) S0[j] = a[K-1-j] % MOD;

        for (int r = 0; r < K; ++r)
            for (int c = 0; c < K; ++c)
                base[r][c] = M[r][c];
        for (int j = 0; j < K; ++j) result[j] = S0[j];

        while (p > 0) {
            if (p & 1) {
                matvec(base, result, tmpVec);
                for (int j = 0; j < K; ++j) result[j] = tmpVec[j];
            }
            p >>= 1;
            if (p > 0) {
                matmul(base, base, tmpMat);
                for (int r = 0; r < K; ++r)
                    for (int c = 0; c < K; ++c)
                        base[r][c] = tmpMat[r][c];
            }
        }

        printf("%d\n", result[0]);
    }
    return 0;
}
