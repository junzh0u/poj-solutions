// POJ 3607 - Distribution of Toys
// Model: claude-sonnet-5
//
// n distinct toys go to m *identical* classes, every class must end up
// with >= k toys, and every toy must be used (board thread 95291/95292
// confirms "no leftovers allowed"). Count the distributions mod 9973.
// Board thread 98529 also pins the m==0 corner: m==0,n==0 -> 1,
// m==0,n>0 -> 0.
//
// This is "partition n labeled elements into m unlabeled nonempty
// blocks, each block of size >= k". Let dp_k[i][j] be that count for i
// elements / j blocks under threshold k>=1. Splitting on the block that
// contains element i (the largest-indexed element):
//   - if removing element i still leaves that block at size >= k, the
//     block was already valid before i joined, so this is any of the j
//     valid blocks of a dp_k[i-1][j] configuration: j * dp_k[i-1][j].
//   - otherwise the block's size without i is exactly k-1 (it can't be
//     less, since including i it must already be >= k), so i is one of
//     the k founding members of a brand-new block; choose the other
//     k-1 founders from the remaining i-1 elements and place the rest:
//     C(i-1,k-1) * dp_k[i-k][j-1].
// So dp_k[i][j] = j*dp_k[i-1][j] + C(i-1,k-1)*dp_k[i-k][j-1], with
// dp_k[0][0]=1. Verified against the sample (k=2): dp_2[4][2] = 3, and
// against a brute-force set-partition enumerator for all n<=8, m<=5,
// k<=4.
//
// k=0 means classes may be left empty, a different structure (no
// "founding" step needed), handled separately: filling m possibly-
// empty identical classes with n distinct toys is sum_{j=0}^{m} S(n,j),
// the ordinary Stirling numbers of the second kind (the k=1 dp with no
// lower cap on how many classes end up used). k=1 is exactly S(n,m).
// Both reuse one Stirling prefix-sum table precomputed once for all
// n,m <= 1000 in O(n*m).
//
// For k>=2 a naive per-query dp_k table is O(n*m) and with m*k<=n
// required for a nonzero answer, m<=n/k -- but many test cases can
// still add up to too much work if many distinct k values appear. So
// queries are bucketed by k first; each distinct k's table is built
// once, sized only to the max (n,m) actually asked for that k, and
// every query sharing that k is answered from it. Total work across
// all buckets is bounded by sum_{k=2}^{1000} 1000*(1000/k) ~= 7e6,
// independent of how many test cases T there are.

#include <cstdio>
#include <vector>

using std::vector;

const int MOD = 9973;
const int MAXN = 1001;

long fact[MAXN], invfact[MAXN];

long power_mod(long b, long e, long m) {
    long r = 1 % m;
    b %= m;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

long comb(int a, int b) {
    if (b < 0 || a < 0 || b > a) return 0;
    return fact[a] * invfact[b] % MOD * invfact[a - b] % MOD;
}

// prefS[i][j] = sum_{t=0}^{j} S(i,t) mod MOD, ordinary Stirling numbers
// of the second kind (blocks of size >= 1), for 0 <= i,j <= 1000.
static int prefS[MAXN][MAXN];

long S_prev[MAXN], S_curr[MAXN];

void build_stirling_prefix() {
    S_prev[0] = 1;
    prefS[0][0] = 1;
    for (int j = 1; j < MAXN; j++) {
        S_prev[j] = 0;
        prefS[0][j] = 1;
    }
    for (int i = 1; i < MAXN; i++) {
        S_curr[0] = 0;
        prefS[i][0] = 0;
        for (int j = 1; j < MAXN; j++) {
            long v = ((long) j * S_prev[j] + S_prev[j - 1]) % MOD;
            S_curr[j] = v;
            prefS[i][j] = (int) ((prefS[i][j - 1] + v) % MOD);
        }
        for (int j = 0; j < MAXN; j++) S_prev[j] = S_curr[j];
    }
}

long stirling(int n, int m) {
    if (m < 0) return 0;
    if (m == 0) return prefS[n][0];
    long v = prefS[n][m] - prefS[n][m - 1];
    if (v < 0) v += MOD;
    return v;
}

// Table for the k currently being processed (k >= 2): dp2[i][j],
// i up to 1000, j up to 500 (floor(1000/2), the largest possible j
// range across any k >= 2).
static int dp2[MAXN][501];

int main() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = fact[i - 1] * i % MOD;
    invfact[MAXN - 1] = power_mod(fact[MAXN - 1], MOD - 2, MOD);
    for (int i = MAXN - 1; i > 0; i--) invfact[i - 1] = invfact[i] * i % MOD;

    build_stirling_prefix();

    int T;
    if (scanf("%d", &T) != 1) return 0;

    vector<int> qn(T), qm(T), qk(T), ans(T, 0);
    vector<vector<int> > buckets(MAXN); // buckets[k] = query indices with that k, k>=2

    for (int i = 0; i < T; i++) {
        int n, m, k;
        scanf("%d %d %d", &n, &m, &k);
        qn[i] = n; qm[i] = m; qk[i] = k;
        if (m == 0) {
            ans[i] = (n == 0) ? 1 : 0;
        } else if (k == 0) {
            int cap = m < n ? m : n;
            ans[i] = (int) prefS[n][cap];
        } else if (k == 1) {
            ans[i] = (int) stirling(n, m);
        } else {
            buckets[k].push_back(i);
        }
    }

    for (int k = 2; k < MAXN; k++) {
        if (buckets[k].empty()) continue;
        int Nk = 0, maxm = 0;
        for (size_t t = 0; t < buckets[k].size(); t++) {
            int idx = buckets[k][t];
            if (qn[idx] > Nk) Nk = qn[idx];
            if (qm[idx] > maxm) maxm = qm[idx];
        }
        int capByN = Nk / k;
        int Mk = maxm < capByN ? maxm : capByN;

        // dp2[i][0]
        dp2[0][0] = 1;
        for (int i = 1; i <= Nk; i++) dp2[i][0] = 0;
        for (int j = 1; j <= Mk; j++) {
            dp2[0][j] = 0;
            for (int i = 1; i <= Nk; i++) {
                long v = (long) j * dp2[i - 1][j] % MOD;
                if (i >= k) {
                    long c = comb(i - 1, k - 1);
                    v = (v + c * dp2[i - k][j - 1]) % MOD;
                }
                dp2[i][j] = (int) v;
            }
        }

        for (size_t t = 0; t < buckets[k].size(); t++) {
            int idx = buckets[k][t];
            int n = qn[idx], m = qm[idx];
            ans[idx] = (m <= Mk) ? dp2[n][m] : 0;
        }
    }

    for (int i = 0; i < T; i++) printf("%d\n", ans[i]);
    return 0;
}
