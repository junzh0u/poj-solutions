// POJ 3184 - Finicky Grazers
// Model: claude-sonnet-5
//
// The leximin-optimal way to place N cows in [0,L] keeping their order forces
// the first cow to final position 0 and the last to L (any slack could be
// used to enlarge the smallest gap, which is a leximin improvement), and the
// N-1 gaps to consist of exactly b = L mod (N-1) gaps of size D+1 and
// a = N-1-b gaps of size D, where D = L / (N-1) (integer division). This is
// the unique (up to which slots get D+1) leximin-maximal gap multiset for a
// fixed integer sum L split into N-1 positive integer parts.
//
// We then choose, among all orderings of the multiset {a copies of D, b
// copies of D+1} assigned to the N-1 gap slots in order, the one minimizing
// total |p_i - q_i|, with a DP over (gap index, number of D-gaps used so
// far) -- the running position is a function of those two numbers alone, so
// cost is well defined. Complexity is O(N * min(a+1,b+1)) after pruning
// infeasible states, which meets the time limit even at N=10000.
//
// All quantities fit comfortably in 32-bit int: positions/D/L <= 100000,
// and the accumulated cost is at most N*L <= 1e9 < INT_MAX, so plain int
// suffices (no long long needed, no compiler-support question).
//
// Ambiguity check: is span forced to exactly L (rather than <= L)? Verified
// by the leximin argument above (also matches board hint "position 0 and L
// must have a cow") and by a differential brute force against all C(N-1, b)
// assignments for small N, L.
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

int main() {
    int n, L;
    if (scanf("%d %d", &n, &L) != 2) return 0;
    vector<int> p(n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

    if (n == 1) {
        printf("0\n");
        return 0;
    }

    int D = L / (n - 1);
    int b = L % (n - 1); // gaps of size D+1
    int a = (n - 1) - b; // gaps of size D

    const int INF = 1000000000;
    vector<int> dp(a + 1, INF), ndp(a + 1, INF);
    dp[0] = p[0]; // |p[0] - 0|

    for (int t = 1; t <= n - 1; t++) {
        int lo = t - b; if (lo < 0) lo = 0;
        int hi = t < a ? t : a;
        for (int j = lo; j <= hi; j++) ndp[j] = INF;

        int plo = (t - 1) - b; if (plo < 0) plo = 0;
        int phi = (t - 1) < a ? (t - 1) : a;
        for (int j = plo; j <= phi; j++) {
            if (dp[j] >= INF) continue;
            int cur = dp[j];
            // use a D-gap now: new j' = j+1
            if (j + 1 <= a) {
                int pos = (j + 1) * D + (t - (j + 1)) * (D + 1);
                int diff = p[t] - pos; if (diff < 0) diff = -diff;
                int c = cur + diff;
                if (c < ndp[j + 1]) ndp[j + 1] = c;
            }
            // use a (D+1)-gap now: j stays
            {
                int pos = j * D + (t - j) * (D + 1);
                int diff = p[t] - pos; if (diff < 0) diff = -diff;
                int c = cur + diff;
                if (c < ndp[j]) ndp[j] = c;
            }
        }
        swap(dp, ndp);
    }

    printf("%d\n", dp[a]);
    return 0;
}
