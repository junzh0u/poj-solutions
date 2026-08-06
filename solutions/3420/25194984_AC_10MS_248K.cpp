// POJ 3420 - Quad Tiling
// Model: claude-sonnet-5
// Approach: number of domino tilings of a 4xN board follows the linear
// recurrence f(n) = f(n-1) + 5*f(n-2) + f(n-3) - f(n-4) for n>=4, with
// f(0)=1, f(1)=1, f(2)=5, f(3)=11 (matches samples and the problem's
// discuss board). N can be up to 1e9, so f(N) mod M is computed with
// 4x4 companion-matrix exponentiation (mod M, M need not be prime, so
// no inverses are used). Ambiguity noted on the board: M can be as
// small as 1, and even the base cases (n=0..3) must be reduced mod M,
// not just the recurrence steps.
#include <cstdio>
#include <cstring>

typedef long long ll;

struct Mat {
    ll a[4][4];
};

static ll MOD;

Mat matMul(const Mat &A, const Mat &B) {
    Mat C;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ll s = 0;
            for (int k = 0; k < 4; ++k) {
                s += A.a[i][k] * B.a[k][j];
            }
            s %= MOD;
            if (s < 0) s += MOD;
            C.a[i][j] = s;
        }
    }
    return C;
}

Mat matPow(Mat base, ll e) {
    Mat result;
    memset(result.a, 0, sizeof(result.a));
    for (int i = 0; i < 4; ++i) result.a[i][i] = 1 % MOD;
    while (e > 0) {
        if (e & 1) result = matMul(result, base);
        base = matMul(base, base);
        e >>= 1;
    }
    return result;
}

int main() {
    ll n, m;
    while (scanf("%lld %lld", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        MOD = m;
        ll base4[4] = { 11 % MOD, 5 % MOD, 1 % MOD, 1 % MOD }; // a(3),a(2),a(1),a(0)
        ll ans;
        if (n == 0) {
            ans = 1 % MOD;
        } else if (n == 1) {
            ans = 1 % MOD;
        } else if (n == 2) {
            ans = 5 % MOD;
        } else if (n == 3) {
            ans = 11 % MOD;
        } else {
            Mat T;
            memset(T.a, 0, sizeof(T.a));
            T.a[0][0] = 1; T.a[0][1] = 5; T.a[0][2] = 1; T.a[0][3] = -1;
            T.a[1][0] = 1;
            T.a[2][1] = 1;
            T.a[3][2] = 1;
            Mat P = matPow(T, n - 3);
            ll s = 0;
            for (int k = 0; k < 4; ++k) s += P.a[0][k] * base4[k];
            s %= MOD;
            if (s < 0) s += MOD;
            ans = s;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
