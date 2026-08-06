// POJ 1821 - Fence
// Model: claude-sonnet-5
// Approach: Sort the K workers by their seat position S (all distinct).
// dp[i][j] = best total income using only the first i (sorted) workers,
// restricted to planks 1..j.  Recurrence (j from 1..N, for worker i with
// (L,P,S)):
//   dp[i][j] = max( dp[i][j-1],                      // plank j left unpainted
//                   dp[i-1][j],                       // worker i unused
//                   max_{p in [max(0,j-L), S-1]} dp[i-1][p] + P*(j-p) )
//                                                      // worker i paints (p,j]
// The last term requires j >= S (interval must contain S) and is only
// considered while p+1<=S<=j and j-p<=L.  For fixed worker i the window's
// right bound (S-1) never changes while its left bound (j-L) only grows as
// j increases, so a monotonic deque over p=0..S-1 (values dp[i-1][p]-P*p)
// answers every j in O(1) amortized, giving O(N*K) total.
// Ambiguity check: the discuss board confirms a worker's painted interval
// need not start at S -- it only has to contain S, matching this reading of
// the statement ("he may paint only a compact interval... interval should
// contain the Si plank").
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct Worker {
    int L, P, S;
};

bool cmpS(const Worker& a, const Worker& b) { return a.S < b.S; }

int main() {
    int N, K;
    if (scanf("%d %d", &N, &K) != 2) return 0;
    vector<Worker> w(K);
    for (int i = 0; i < K; ++i) {
        scanf("%d %d %d", &w[i].L, &w[i].P, &w[i].S);
    }
    sort(w.begin(), w.end(), cmpS);

    vector<long long> prevDp(N + 1, 0), curr(N + 1, 0);
    vector<int> dqP(N + 2);
    vector<long long> dqVal(N + 2);

    for (int i = 0; i < K; ++i) {
        int L = w[i].L, P = w[i].P, S = w[i].S;

        // Build monotonic deque over p = 0 .. S-1 using prevDp.
        int head = 0, tail = -1;
        for (int p = 0; p <= S - 1; ++p) {
            long long val = prevDp[p] - (long long)P * p;
            while (tail >= head && dqVal[tail] <= val) tail--;
            dqP[++tail] = p;
            dqVal[tail] = val;
        }

        curr[0] = 0;
        int hi = min(N, S + L - 1);
        for (int j = 1; j <= N; ++j) {
            long long base = max(curr[j - 1], prevDp[j]);
            if (j >= S && j <= hi) {
                int lowbound = max(0, j - L);
                while (tail >= head && dqP[head] < lowbound) head++;
                if (tail >= head) {
                    long long candidate = dqVal[head] + (long long)P * j;
                    if (candidate > base) base = candidate;
                }
            }
            curr[j] = base;
        }
        prevDp.swap(curr);
    }

    printf("%lld\n", prevDp[N]);
    return 0;
}
