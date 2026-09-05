// POJ 3900 - The Robbery
// Model: claude-sonnet-5
// Approach: bounded knapsack via an adaptive incremental Pareto-frontier
// merge with a memory-capped fallback. N <= 15 box types, box k holds k
// identical diamonds (weight Wk, cost Ck each); M, W, C up to 1e9 rules out
// a weight-indexed DP, and the theoretical combinatorial space (product of
// (k+1) over all boxes, up to 16! for N=15) rules out storing every
// achievable (weight,cost) pair. Instead maintain a frontier of (weight,
// cost) points sorted by weight with strictly increasing cost (any point
// off this Pareto front is dominated and can't be part of an optimal
// solution): for each box, convolve every frontier point with every count
// 0..k of that box (dropping sums exceeding M), then re-sort and re-prune.
// This alone (no cap) MLE'd on the real judge data (run 25202506): some
// hidden test evidently has far less dominance than the boards' own posted
// "hard" case (which this collapses to a frontier of ~5000 points and
// solves in milliseconds), so the frontier isn't bounded in general. Fix:
// before each merge step, check the worst-case size the next frontier could
// reach (current size * (count+1)); if it would exceed a safe budget
// (1.5M points ~ 24MB, leaving headroom under the 64MB limit even with the
// transient buffers of one merge step), stop merging and instead stream
// every remaining box's combinations by brute recursion, binary-searching
// the (now fixed, bounded) frontier for the best completion of each. This
// keeps memory bounded unconditionally while still running in milliseconds
// whenever the data has real dominance (verified against the board's case,
// uncorrelated random data, and the official sample), and remains within a
// few seconds even for a deliberately built worst-case instance (strongly
// correlated cost = weight + const, capacity at half the maximum, zero
// dominance) checked locally across multiple seeds. No statement
// ambiguity: box k's k diamonds are identical (same weight/cost per
// diamond), confirmed by the sample.
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;

int n;
ll M;
ll W[20], C[20], CNT[20];
vector<pll> frontier;
ll gAns;

void streamRest(int idx, ll w, ll c) {
    if (idx == n) {
        if (w > M) return;
        ll rem = M - w;
        int lo=0, hi=(int)frontier.size()-1, pos=-1;
        while (lo<=hi) {
            int mid=(lo+hi)/2;
            if (frontier[mid].first <= rem) { pos=mid; lo=mid+1; } else hi=mid-1;
        }
        ll addc = (pos>=0) ? frontier[pos].second : 0;
        ll tot = c + addc;
        if (tot > gAns) gAns = tot;
        return;
    }
    ll cnt = CNT[idx], w_i = W[idx], c_i = C[idx];
    for (ll t = 0; t <= cnt; t++) {
        ll nw = w + t * w_i;
        if (nw > M) break;
        streamRest(idx+1, nw, c + t * c_i);
    }
}

int main(){
    int T; scanf("%d",&T);
    const ll NEXT_CAP = 1500000; // cap on the raw "next" list size per merge step
    while(T--){
        scanf("%d %lld",&n,&M);
        for(int i=0;i<n;i++) scanf("%lld",&W[i]);
        for(int i=0;i<n;i++) scanf("%lld",&C[i]);
        for(int i=0;i<n;i++) CNT[i]=i+1;

        frontier.clear();
        frontier.push_back(pll(0,0));

        int idx = 0;
        for (; idx < n; idx++) {
            ll cnt = CNT[idx];
            // predicted worst-case size of the next raw list
            double predicted = (double)frontier.size() * (double)(cnt + 1);
            if (predicted > (double)NEXT_CAP) break; // stop merging; stream the rest
            ll w_i = W[idx], c_i = C[idx];
            vector<pll> next;
            next.reserve(frontier.size() * (size_t)(cnt + 1));
            for (size_t j = 0; j < frontier.size(); j++) {
                ll w = frontier[j].first, c = frontier[j].second;
                for (ll t = 0; t <= cnt; t++) {
                    ll nw = w + t * w_i;
                    if (nw > M) break;
                    next.push_back(pll(nw, c + t * c_i));
                }
            }
            sort(next.begin(), next.end());
            vector<pll> pruned;
            pruned.reserve(next.size());
            ll best = -1;
            for (size_t j = 0; j < next.size(); j++) {
                if (next[j].second > best) {
                    pruned.push_back(next[j]);
                    best = next[j].second;
                }
            }
            frontier.swap(pruned);
        }

        if (idx == n) {
            // fully merged via incremental Pareto frontier
            ll ans = frontier.empty() ? 0 : frontier.back().second;
            printf("%lld\n", ans);
        } else {
            // stream boxes [idx, n) against the (bounded) frontier
            gAns = 0;
            streamRest(idx, 0, 0);
            printf("%lld\n", gAns);
        }
    }
    return 0;
}
