// POJ 1180 - Batch Scheduling
// Model: claude-opus-5
//
// Approach: suffix DP + convex hull trick (O(N)).
//
// Split the jobs into consecutive batches. Work from the back: let
//   sumT[i] = T_i + ... + T_N, sumF[i] = F_i + ... + F_N,
//   dp[i]   = minimum cost of jobs i..N, measured as if the batch starting
//             at i starts at time 0.
// If the first batch is {i..j-1}, it takes S + (sumT[i] - sumT[j]) time, and
// that whole span delays *every* remaining job i..N (the batch's own jobs
// output at its end, later batches start after it), so it adds
//   (S + sumT[i] - sumT[j]) * sumF[i]
// to the total. Hence
//   dp[i] = min_{i<j<=N+1} { dp[j] - sumT[j]*sumF[i] } + (sumT[i] + S)*sumF[i],
//   dp[N+1] = 0, answer dp[1].
//
// The bracket is a minimum over lines y = m*x + b with m = -sumT[j],
// b = dp[j], queried at x = sumF[i]. Processing i from N down to 1 inserts
// lines with strictly decreasing slope (T_i >= 1) and queries at strictly
// increasing x (F_i >= 1), so a monotone convex hull with a walking pointer
// works: append at the back, never pop from the front, and clamp the pointer
// after a pop. O(N) total.
//
// Notes:
//  - The discuss board reports plain O(N^2) DP timing out here, and
//    std::deque blowing the 10000K memory limit, so the hull lives in two
//    fixed long long arrays.
//  - Costs reach ~(10^6 + 50) * 10^6 ~ 10^12, so long long throughout; the
//    hull's cross-multiplied comparison stays under 10^18.
//  - Output goes through std::cout rather than printf("%lld") because POJ's
//    language 4 (C++) is an old MSVC where %lld is not portable.
//
// No statement ambiguity: the sample partition {1,2}{3}{4,5} reproduces the
// stated output times (5,5,10,14,14) and total 153 under this model.

#include <cstdio>
#include <iostream>

const int MAXN = 10005;

int T[MAXN], F[MAXN];
long long sumT[MAXN], sumF[MAXN], dp[MAXN];
long long hullM[MAXN], hullB[MAXN];

int main() {
    int n, S;
    if (scanf("%d", &n) != 1) return 0;
    if (scanf("%d", &S) != 1) return 0;
    for (int i = 1; i <= n; ++i) {
        if (scanf("%d %d", &T[i], &F[i]) != 2) return 0;
    }

    sumT[n + 1] = 0;
    sumF[n + 1] = 0;
    for (int i = n; i >= 1; --i) {
        sumT[i] = sumT[i + 1] + T[i];
        sumF[i] = sumF[i + 1] + F[i];
    }

    int sz = 0, ptr = 0;
    dp[n + 1] = 0;
    hullM[sz] = -sumT[n + 1];  /* line for j = n+1 */
    hullB[sz] = dp[n + 1];
    ++sz;

    for (int i = n; i >= 1; --i) {
        long long x = sumF[i];
        if (ptr >= sz) ptr = sz - 1;
        while (ptr + 1 < sz &&
               hullM[ptr + 1] * x + hullB[ptr + 1] <=
                   hullM[ptr] * x + hullB[ptr]) {
            ++ptr;
        }
        dp[i] = hullM[ptr] * x + hullB[ptr] + (sumT[i] + (long long)S) * sumF[i];

        if (i > 1) {  /* dp[1] is never used as a hull line */
            long long m3 = -sumT[i], b3 = dp[i];
            while (sz >= 2) {
                long long m1 = hullM[sz - 2], b1 = hullB[sz - 2];
                long long m2 = hullM[sz - 1], b2 = hullB[sz - 1];
                /* drop line 2 when (b2-b1)/(m1-m2) >= (b3-b2)/(m2-m3) */
                if ((b2 - b1) * (m2 - m3) >= (b3 - b2) * (m1 - m2)) --sz;
                else break;
            }
            hullM[sz] = m3;
            hullB[sz] = b3;
            ++sz;
            if (ptr >= sz) ptr = sz - 1;
        }
    }

    std::cout << dp[1] << std::endl;
    return 0;
}
