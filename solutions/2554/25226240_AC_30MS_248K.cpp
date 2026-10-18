// POJ 2554 - Fixed Partition Contest Management
// Model: gpt-6-astra
// For each member and subset, shortest processing time first minimizes the
// sum of completion times. Partition the problems among members by subset DP,
// retaining each chosen subset to reconstruct the actual schedule.
// A member uses the last breakpoint at or below its full intelligence;
// faster members cannot choose an earlier (possibly faster) running time.
#include <cstdio>
#include <algorithm>
#include <utility>
using namespace std;
const double INF = 1e100;
double duration[3][10], cost[3][1024], dp[4][1024];
int take[4][1024];
int main() {
    int m,n,tc=0;
    while (scanf("%d%d", &m, &n)==2 && (m || n)) {
        double brightness[3];
        for(int j=0;j<m;++j) scanf("%lf", &brightness[j]);
        for(int i=0;i<n;++i) {
            for(int j=0;j<m;++j) duration[j][i]=-1;
            int k; scanf("%d", &k);
            while(k--) {
                double s,t; scanf("%lf%lf", &s,&t);
                for(int j=0;j<m;++j)
                    if(brightness[j]>=s) duration[j][i]=t;
            }
        }
        int limit=1<<n;
        for(int j=0;j<m;++j) {
            pair<double,int> order[10];
            for(int i=0;i<n;++i) order[i]=make_pair(duration[j][i],i);
            sort(order,order+n);
            for(int mask=0;mask<limit;++mask) {
                double elapsed=0,total=0;
                for(int i=0;i<n;++i) if(mask&(1<<order[i].second)) {
                    if(order[i].first<0) {total=INF; break;}
                    elapsed+=order[i].first; total+=elapsed;
                }
                cost[j][mask]=total;
            }
        }
        for(int mask=0;mask<limit;++mask) dp[0][mask]=mask?INF:0;
        for(int j=1;j<=m;++j) for(int mask=0;mask<limit;++mask) {
            dp[j][mask]=INF;
            for(int sub=mask;;sub=(sub-1)&mask) {
                double value=dp[j-1][mask^sub]+cost[j-1][sub];
                if(value<dp[j][mask]) {dp[j][mask]=value;take[j][mask]=sub;}
                if(!sub) break;
            }
        }
        int member[10],mask=limit-1;
        double start[10],finish[10];
        for(int j=m;j>=1;--j) {
            int sub=take[j][mask]; mask^=sub;
            pair<double,int> order[10]; int count=0;
            for(int i=0;i<n;++i) if(sub&(1<<i))
                order[count++]=make_pair(duration[j-1][i],i);
            sort(order,order+count);
            double elapsed=0;
            for(int i=0;i<count;++i) {
                int p=order[i].second; member[p]=j; start[p]=elapsed;
                elapsed+=order[i].first; finish[p]=elapsed;
            }
        }
        printf("Case %d\nAverage solution time = %.2f\n",++tc,dp[m][limit-1]/n+1e-8);
        for(int i=0;i<n;++i)
            printf("Problem %d is solved by member %d from %.0f to %.0f\n",
                   i+1,member[i],start[i],finish[i]);
        printf("\n");
    }
    return 0;
}
