// POJ 2754 - Similarity of necklaces 2
// Model: claude-sonnet-5
//
// Reformulate: let x[i] = Table[i] - Low[i], so x[i] in [0, D[i]] with
// D[i] = Up[i]-Low[i] (>=1 since Low[i] < Up[i]). The linear constraint
// sum(Multi[i]*Table[i]) = 0 becomes sum(Multi[i]*x[i]) = C where
// C = -sum(Multi[i]*Low[i]). The objective sum(Pairs[i]*Table[i]) becomes
// sum(Pairs[i]*x[i]) + base, base = sum(Pairs[i]*Low[i]).
//
// This is a bounded knapsack with an EXACT target weight: choose integer
// x[i] in [0,D[i]], weight Multi[i] (1..20), value Pairs[i] per unit,
// maximize total value subject to total weight == C. Since D[i]<=50,
// Multi[i]<=20 and M<=200, the maximum achievable weight sum Smax is at
// most 200*20*50 = 200000. All x[i]>=0 with positive weights, so every
// partial sum along the way to a final total of C is itself <= C (it can
// only grow from there); states above C can therefore never contribute to
// dp[C] and are pruned by capping the dp array at C instead of Smax (only
// dp[C] is ever read back), which roughly halves the worst case in
// practice (the configuration that maximizes Smax while forcing C==Smax
// caps out at Smax==C==100000, not 200000). Each item is folded in with
// the standard monotonic-deque bounded-knapsack trick (split state space
// by residue mod Multi[i], slide a max-window of size D[i]+1 over each
// residue chain) for O(C) work per item instead of O(C*D[i]).
//
// No ambiguity in the statement itself once the two condition images are
// read: the "similarity of necklaces" framing is just narrative, the real
// problem is the linear-programming-with-one-equality-constraint above.
// Verified against both sample cases by hand (item-by-item reasoning) and
// against a brute-force meet-in-the-middle/DP-by-enumeration reference on
// random small cases (M<=6, small ranges) before submitting.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const long long NEG = -1000000000000LL; // -1e12, far below any real dp value

const int MAXS = 200005;
long long dp[MAXS];
long long h[MAXS];
int dq[MAXS];

int Pairs_[205], Multi_[205], Low_[205], Up_[205], D_[205];

int main(){
    int M;
    while (scanf("%d", &M) == 1) {
        long long base = 0;
        long long C = 0;
        long long Smax = 0;
        for (int i = 0; i < M; i++) {
            scanf("%d %d %d %d", &Pairs_[i], &Multi_[i], &Low_[i], &Up_[i]);
            D_[i] = Up_[i] - Low_[i];
            base += (long long)Pairs_[i] * Low_[i];
            C -= (long long)Multi_[i] * Low_[i];
            Smax += (long long)Multi_[i] * D_[i];
        }

        long long ans = 0;
        if (C >= 0 && C <= Smax) {
            long long lim = C; // states above the target can never help reach it

            for (long long s = 0; s <= lim; s++) dp[s] = NEG;
            dp[0] = 0;

            for (int i = 0; i < M; i++) {
                int w = Multi_[i];
                int Dd = D_[i];
                long long p = Pairs_[i];
                for (int r = 0; r < w && r <= lim; r++) {
                    int cnt = 0;
                    for (long long s = r; s <= lim; s += w, cnt++) {
                        h[cnt] = dp[s] - p * (long long)cnt;
                    }
                    int dqHead = 0, dqTail = 0;
                    long long s = r;
                    for (int t = 0; t < cnt; t++, s += w) {
                        while (dqTail > dqHead && h[dq[dqTail - 1]] <= h[t]) dqTail--;
                        dq[dqTail++] = t;
                        while (dq[dqHead] < t - Dd) dqHead++;
                        dp[s] = h[dq[dqHead]] + p * (long long)t;
                    }
                }
            }

            if (dp[lim] > NEG / 2) ans = base + dp[lim];
        }
        printf("%lld\n", ans);
    }
    return 0;
}
