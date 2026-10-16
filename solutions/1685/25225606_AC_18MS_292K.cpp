// POJ 1685 - Color Tunnels
// Model: gpt-6-astra
// Layered shortest paths: retain both possible finishing endpoints of each
// tunnel after every required color. Each transition includes travel to the
// opposite endpoint and traversal of the entire tunnel. Crossing a segment
// alone does not paint; tunnels may be reused, including for repeated colors.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y; } p[122];
double d[122][122], dp[122], ndp[122];
int color[61], seq[31];
double distance(Point a,Point b) { double x=a.x-b.x,y=a.y-b.y; return sqrt(x*x+y*y); }
int main() {
    int tests; if(scanf("%d",&tests)!=1) return 0;
    while(tests--) {
        Point dest;
        scanf("%lf%lf%lf%lf",&p[120].x,&p[120].y,&dest.x,&dest.y);
        int m,n; scanf("%d",&m);
        for(int i=0;i<m;++i) scanf("%d",&seq[i]);
        scanf("%d",&n);
        for(int i=0;i<n;++i)
            scanf("%lf%lf%lf%lf%d",&p[2*i].x,&p[2*i].y,&p[2*i+1].x,&p[2*i+1].y,&color[i]);
        for(int i=0;i<2*n;++i) for(int j=0;j<2*n;++j) d[i][j]=distance(p[i],p[j]);
        for(int j=0;j<2*n;++j) dp[j]=color[j/2]==seq[0]?distance(p[120],p[j^1])+d[j][j^1]:1e100;
        for(int k=1;k<m;++k) {
            for(int j=0;j<2*n;++j) {
                ndp[j]=1e100;
                if(color[j/2]==seq[k]) for(int i=0;i<2*n;++i)
                    ndp[j]=min(ndp[j],dp[i]+d[i][j^1]+d[j][j^1]);
            }
            for(int j=0;j<2*n;++j) dp[j]=ndp[j];
        }
        double ans=1e100;
        for(int j=0;j<2*n;++j) ans=min(ans,dp[j]+distance(p[j],dest));
        printf("%.10f\n",ans);
    }
    return 0;
}
