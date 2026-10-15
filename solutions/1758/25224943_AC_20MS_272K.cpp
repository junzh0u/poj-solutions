// POJ 1758 - Frontier
// Model: gpt-6.1-sol
// Keep only directed chords with every monument strictly to their right.
// An increasing-index shortest path plus its closing chord gives each cycle.
// With no monuments, enumerate non-collinear triangles: collinear towers
// are allowed, but the revised frontier must have positive area.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { int x,y; };
int cross(Point a, Point b, Point c) {
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
Point p[50], q[1000];
double d[50][50], dp[50];
bool ok[50][50];
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    for(int i=0;i<n;i++) scanf("%d%d",&p[i].x,&p[i].y);
    for(int i=0;i<m;i++) scanf("%d%d",&q[i].x,&q[i].y);
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) {
        double dx=p[i].x-p[j].x,dy=p[i].y-p[j].y;
        d[i][j]=sqrt(dx*dx+dy*dy);
        ok[i][j]=(i!=j);
        for(int k=0;k<m;k++) if(cross(p[i],p[j],q[k])>=0) {
            ok[i][j]=false; break;
        }
    }
    double ans=1e100;
    if(m==0) {
        for(int i=0;i<n;i++) for(int j=i+1;j<n;j++)
            for(int k=j+1;k<n;k++) if(cross(p[i],p[j],p[k])!=0)
                ans=min(ans,d[i][j]+d[j][k]+d[k][i]);
    } else {
        for(int s=0;s<n;s++) {
            for(int i=0;i<n;i++) dp[i]=1e100;
            dp[s]=0;
            for(int j=s+1;j<n;j++) {
                for(int i=s;i<j;i++) if(ok[i][j])
                    dp[j]=min(dp[j],dp[i]+d[i][j]);
                if(ok[j][s]) ans=min(ans,dp[j]+d[j][s]);
            }
        }
    }
    printf("%.2f\n",ans);
    return 0;
}
