// POJ 3726 - Windy's ABC
// Model: claude-sonnet-5
//
// S consists only of letters A/B/C. A "copy" is a rearrangement (multiset
// permutation) T of S. For a letter c with occurrences at sorted positions
// p_1<...<p_k in S, the copy assigns that letter to some sorted set of
// positions q_1<...<q_k in T; the copy is good iff |p_i - q_i| <= R_c for
// every i and every letter c (this order-preserving matching is provable
// optimal by an exchange argument -- any other matching between same-letter
// occurrences only increases the max distance -- and the discuss board
// (message 116305) confirms distance is defined per matched occurrence
// pair, settling the RA-is-sum-vs-single-distance ambiguity raised in
// message 116953 in favor of "single occurrence, not summed").
//
// Build T by deciding, for each target position t = 1..n in order, which
// letter goes there. The i-th time letter c is placed (i-th occurrence of c
// in T, left to right) must correspond to the i-th smallest original
// position p_i of c, since positions filled left-to-right are automatically
// increasing. So state (i,j,k) = number of A/B/C placed so far, and moving
// to position t=i+j+k+1 by placing the next A/B/C is legal iff
// |p_{i+1}-(t)| <= R_c. dp[i][j][k] = number of ways to have validly built
// the first i+j+k positions using i A's, j B's, k C's; transitions come
// from dp[i-1][j][k], dp[i][j-1][k], dp[i][j][k-1]. Answer is
// dp[countA][countB][countC], taken modulo 20090305.
//
// Memory is rolled over the i dimension (two 2D layers of size
// (countB+1)x(countC+1)) rather than materializing the full 3D cube, since
// the discuss board (message 118858) reports the naive 500^3 array blowing
// the 64MB limit.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static const long long MOD = 20090305;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    static char buf[600];
    while (T--) {
        long long RA, RB, RC;
        scanf("%lld %lld %lld", &RA, &RB, &RC);
        scanf("%s", buf);
        int n = (int)strlen(buf);
        vector<int> pA, pB, pC;
        for (int i = 0; i < n; i++) {
            int pos = i + 1;
            if (buf[i] == 'A') pA.push_back(pos);
            else if (buf[i] == 'B') pB.push_back(pos);
            else pC.push_back(pos);
        }
        int countA = (int)pA.size(), countB = (int)pB.size(), countC = (int)pC.size();

        vector<vector<long long> > prevLayer(countB + 1, vector<long long>(countC + 1, 0));
        vector<vector<long long> > curLayer(countB + 1, vector<long long>(countC + 1, 0));

        for (int i = 0; i <= countA; i++) {
            for (int j = 0; j <= countB; j++) {
                for (int k = 0; k <= countC; k++) {
                    if (i == 0 && j == 0 && k == 0) {
                        curLayer[j][k] = 1;
                        continue;
                    }
                    long long val = 0;
                    int t = i + j + k;
                    if (i > 0) {
                        int diff = pA[i - 1] - t;
                        if (diff < 0) diff = -diff;
                        if (diff <= RA) val += prevLayer[j][k];
                    }
                    if (j > 0) {
                        int diff = pB[j - 1] - t;
                        if (diff < 0) diff = -diff;
                        if (diff <= RB) val += curLayer[j - 1][k];
                    }
                    if (k > 0) {
                        int diff = pC[k - 1] - t;
                        if (diff < 0) diff = -diff;
                        if (diff <= RC) val += curLayer[j][k - 1];
                    }
                    curLayer[j][k] = val % MOD;
                }
            }
            prevLayer.swap(curLayer);
        }
        printf("%lld\n", prevLayer[countB][countC]);
    }
    return 0;
}
