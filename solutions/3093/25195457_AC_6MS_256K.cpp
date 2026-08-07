// POJ 3093 - Margaritas on the River Walk
// Model: claude-sonnet-5
// Approach: sort vendor prices ascending. A "maximal" combination is
// determined entirely by which vendor is the SMALLEST-priced one that was
// left out: if vendors 1..k-1 (sorted) are all bought and vendor k (price
// p_k) is the first one skipped, the combination is maximal iff the final
// leftover money D - total is in (D - p_k, D - P(k-1)], where P(k-1) is the
// prefix sum of the k-1 cheapest vendors, because the leftover must be less
// than every skipped vendor's price and p_k is the smallest of them.
// If nothing is skipped (everything bought), the combination is maximal
// automatically when its total is <= D.
// For each k this reduces to counting subset sums, over the remaining
// vendors k+1..V, that land the final total in that half-open range; the
// suffix subset-sum distribution is built incrementally as k decreases
// (standard 0/1 knapsack counting), giving O(V*D) per dataset instead of
// O(V^2*D) or worse.
// Board note (yzhw/longxiaozhi/407811696 threads): buying NOTHING never
// counts as a combination, even though it technically satisfies the
// "leftover < every skipped price" rule whenever D is less than every
// vendor's price. Their test data (V=29,D=1,prices 2..30 -> 0) pins this
// down. In the DP this spurious case appears only as the k=1, T=0 term
// (skip the globally cheapest vendor, buy nothing else), and only when
// D < min price; it is subtracted off explicitly at the end.
#include <cstdio>
#include <algorithm>
using namespace std;

typedef unsigned long long u64;

int main(){
    int T;
    if(scanf("%d",&T)!=1) return 0;
    for(int tc=1; tc<=T; ++tc){
        int V, D;
        scanf("%d %d", &V, &D);
        static int p[35];
        for(int i=1;i<=V;i++) scanf("%d", &p[i]);
        sort(p+1, p+V+1);

        static long long P[35];
        P[0]=0;
        for(int i=1;i<=V;i++) P[i]=P[i-1]+p[i];

        u64 ans=0;
        if(P[V] <= D) ans += 1;

        static u64 dp[1005];
        for(int t=0;t<=D;t++) dp[t]=0;
        dp[0]=1;
        static u64 pre[1006];
        for(int k=V; k>=1; --k){
            pre[0]=dp[0];
            for(int t=1;t<=D;t++) pre[t]=pre[t-1]+dp[t];

            long long high = (long long)D - P[k-1];
            if(high >= 0){
                long long lo = high - p[k]; // exclusive lower bound
                long long hi = high;        // inclusive upper bound, always <= D
                u64 sumHigh = pre[hi];
                u64 sumLow = (lo>=0) ? pre[lo] : 0;
                ans += sumHigh - sumLow;
            }

            for(int t=D; t>=p[k]; --t){
                dp[t]+=dp[t-p[k]];
            }
        }

        if(V>=1 && p[1] > D){
            ans -= 1;
        }

        printf("%d %llu\n", tc, ans);
    }
    return 0;
}
