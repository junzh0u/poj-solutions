// POJ 2433 - Landscaping
// Model: claude-sonnet-5
//
// Build the min-Cartesian tree of the elevation array by recursively
// splitting each range at the position of its minimum element (the
// "valley"). Leaves of this tree are exactly the original peaks. For an
// internal node with valley height h and children L, R, we may either keep
// a child's landscape reduced to some number of surviving peaks (its own
// recursive DP), or flatten the entire child subtree down to height h so it
// contributes 0 extra peaks (cost = sum(a[i]-h) over that subtree, computed
// directly from prefix sums). dp[node][j] ("at most j peaks") is the
// knapsack-style min-plus merge of the two children's option lists,
// followed by a prefix-min to keep the "at most" semantics. Every table is
// capped at K entries, which bounds the whole tree-knapsack to O(N*K).
//
// The true root needs one more candidate not covered by the recursive
// merge: flattening BOTH children away simultaneously (down to the global
// minimum) makes the whole array one flat block, which is exactly 1 peak
// only at the top level (a subtree flattened to 0 "extra" peaks is meant to
// be swallowed by an ancestor's plateau, but the true root has no ancestor,
// so that state means "the whole thing is one peak"). We add this
// candidate explicitly and take the min against the recursive dp[root].
//
// Ambiguity check: board discussion (message 30045 etc.) confirms peaks
// can never be reduced to 0, consistent with a flattened whole array
// still counting as exactly 1 peak (matches K>=1 in the constraints).
// Verified against board test case (message 196104): a=[4,1,3,2,3,1],
// K=1 -> expected 4 (a posted WA solution gave 2 on this case).

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
// N<=1000, elevations<=1,000,000, so any sum/volume fits comfortably in a
// 32-bit int (max ~1e9 well under INT_MAX); plain int avoids any
// long-long/%lld portability question across POJ's compilers.
typedef int ll;
const ll INF = 2000000000;

int N, K;
ll a[1005], S[1005];

ll flattenCost(int l, int r, ll h) {
    if (l > r) return 0;
    return (S[r] - S[l - 1]) - h * (ll)(r - l + 1);
}

vector<ll> solve(int l, int r) {
    if (l == r) {
        vector<ll> dp(2, 0);
        return dp; // dp[1] = 0
    }
    int p = l;
    for (int i = l + 1; i <= r; ++i) if (a[i] < a[p]) p = i;
    ll h = a[p];
    bool leftEmpty = (p == l), rightEmpty = (p == r);
    if (leftEmpty && !rightEmpty) return solve(p + 1, r);
    if (rightEmpty && !leftEmpty) return solve(l, p - 1);

    vector<ll> L = solve(l, p - 1);
    vector<ll> R = solve(p + 1, r);
    int capL = (int)L.size() - 1, capR = (int)R.size() - 1;
    int cap = min(capL + capR, K);

    vector<ll> Lopt(capL + 1), Ropt(capR + 1);
    Lopt[0] = flattenCost(l, p - 1, h);
    for (int i = 1; i <= capL; ++i) Lopt[i] = L[i];
    Ropt[0] = flattenCost(p + 1, r, h);
    for (int i = 1; i <= capR; ++i) Ropt[i] = R[i];

    vector<ll> dp(cap + 1, INF);
    for (int j = 1; j <= cap; ++j) {
        int jlMax = min(j, capL);
        for (int jl = 0; jl <= jlMax; ++jl) {
            int jr = j - jl;
            if (jr > capR) continue;
            ll val = Lopt[jl] + Ropt[jr];
            if (val < dp[j]) dp[j] = val;
        }
    }
    for (int j = 2; j <= cap; ++j) if (dp[j - 1] < dp[j]) dp[j] = dp[j - 1];
    return dp;
}

int main() {
    scanf("%d %d", &N, &K);
    for (int i = 1; i <= N; ++i) {
        scanf("%d", &a[i]);
        S[i] = S[i - 1] + a[i];
    }
    vector<ll> rootDP = solve(1, N);
    int cap = (int)rootDP.size() - 1;
    ll ans = rootDP[cap];

    ll globalMin = a[1];
    for (int i = 2; i <= N; ++i) if (a[i] < globalMin) globalMin = a[i];
    ll totalFlatten = flattenCost(1, N, globalMin);
    if (totalFlatten < ans) ans = totalFlatten;

    printf("%d\n", ans);
    return 0;
}
