// POJ 3858 - Hurry Plotter
// Model: gpt-6.1-sol
// For each row and count, minimize the cost with and without returning left.
// Fix the rightmost selected segment, then take the shortest earlier ones.
// A count knapsack combines completed rows; the final row needs no return.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Segment { int y, l, r; };
bool order(const Segment &a, const Segment &b) {
    if (a.y != b.y) return a.y < b.y;
    return a.r < b.r;
}
const int INF = 1000000000;
int main() {
    int n, t;
    while (scanf("%d%d", &n, &t) == 2 && (n || t)) {
        vector<Segment> a(n);
        for (int i=0;i<n;++i) scanf("%d%d%d", &a[i].y,&a[i].l,&a[i].r);
        sort(a.begin(),a.end(),order);
        vector<int> dp(n+1,INF); dp[0]=0;
        int used=0, answer=0;
        for (int s=0;s<n;) {
            int e=s;
            while (e<n && a[e].y==a[s].y) ++e;
            int m=e-s;
            vector<int> back(m+1,INF), stop(m+1,INF), lengths;
            back[0]=stop[0]=0;
            for (int j=s;j<e;++j) {
                int sum=a[j].r-a[j].l;
                for (int k=1;k<=j-s+1;++k) {
                    back[k]=min(back[k],2*a[j].r+sum);
                    stop[k]=min(stop[k],a[j].r+sum);
                    if (k<=j-s) sum+=lengths[k-1];
                }
                lengths.insert(lower_bound(lengths.begin(),lengths.end(),a[j].r-a[j].l),a[j].r-a[j].l);
            }
            vector<int> next(n+1,INF);
            for (int i=0;i<=used;++i) for (int k=0;k<=m;++k) {
                if (dp[i]+stop[k]<=t) answer=max(answer,i+k);
                next[i+k]=min(next[i+k],dp[i]+back[k]);
            }
            dp.swap(next); used+=m; s=e;
        }
        printf("%d\n",answer);
    }
    return 0;
}
