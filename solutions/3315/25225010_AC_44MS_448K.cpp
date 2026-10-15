// POJ 3315 - Roofing It
// Model: gpt-6.1-sol
// Every usable section is a supporting line of the upper convex hull.
// Partition the input points into consecutive groups and minimize the largest
// gap, using at most k supporting lines. The lower envelope of those lines
// is a convex roof and is no higher than the assigned line at each point.
// Collinear hull vertices are removed; extra sections do not improve the roof.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct Point { double x,y; };
Point p[105];
double gap[105][105], cost[105][105], dp[105][105];
int main() {
    int n,k;
    while(scanf("%d%d",&n,&k)==2 && n) {
        for(int i=0;i<n;++i) scanf("%lf%lf",&p[i].x,&p[i].y);
        vector<int> h;
        for(int i=0;i<n;++i) {
            while(h.size()>1) {
                int a=h[h.size()-2],b=h.back();
                double cross=(p[b].x-p[a].x)*(p[i].y-p[b].y)
                            -(p[b].y-p[a].y)*(p[i].x-p[b].x);
                if(cross < -1e-9) break;
                h.pop_back();
            }
            h.push_back(i);
        }
        int m=(int)h.size()-1;
        for(int t=0;t<m;++t) {
            int a=h[t],b=h[t+1];
            double slope=(p[b].y-p[a].y)/(p[b].x-p[a].x);
            for(int i=0;i<n;++i)
                gap[t][i]=max(0.0,p[a].y+slope*(p[i].x-p[a].x)-p[i].y);
        }
        for(int a=0;a<n;++a) for(int b=a;b<n;++b) cost[a][b]=1e100;
        for(int t=0;t<m;++t) for(int a=0;a<n;++a) {
            double v=0;
            for(int b=a;b<n;++b) {
                v=max(v,gap[t][b]);
                cost[a][b]=min(cost[a][b],v);
            }
        }
        k=min(k,m);
        for(int j=0;j<=k;++j) for(int i=0;i<=n;++i) dp[j][i]=1e100;
        dp[0][0]=0;
        for(int j=1;j<=k;++j) {
            dp[j][0]=0;
            for(int i=1;i<=n;++i) for(int a=0;a<i;++a)
                dp[j][i]=min(dp[j][i],max(dp[j-1][a],cost[a][i-1]));
        }
        double ans=floor(dp[k][n]*1000.0+0.5+1e-8)/1000.0;
        printf("%.3f\n",ans);
    }
    return 0;
}
