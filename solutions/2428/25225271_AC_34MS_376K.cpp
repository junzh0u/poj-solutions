// POJ 2428 - Surface Reconstruction
// Model: gpt-6.1-sol
// Normalize polygon orientations, then cut the cyclic triangle strip at a
// bridge from the first vertex to every vertex of the other polygon.
// A grid shortest path adds exactly one polygon boundary per triangle.
// The input orders may differ; no separate 3-D intersection test is imposed.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y; };
Point a[101],b[101];
double ca[100][100],cb[100][100],dp[101][101];
void orient(Point *p,int n) {
    double s=0;
    for(int i=0;i<n;++i) s+=p[i].x*p[(i+1)%n].y-p[i].y*p[(i+1)%n].x;
    if(s<0) reverse(p,p+n);
}
double area(Point p,Point q,Point r) {
    double x=q.x-p.x,y=q.y-p.y;
    double cross=x*(r.y-p.y)-y*(r.x-p.x);
    return 0.5*sqrt(100*(x*x+y*y)+cross*cross);
}
int main() {
    int n,m;
    if(scanf("%d",&n)!=1) return 0;
    for(int i=0;i<n;++i) scanf("%lf%lf",&a[i].x,&a[i].y);
    scanf("%d",&m);
    for(int i=0;i<m;++i) scanf("%lf%lf",&b[i].x,&b[i].y);
    orient(a,n); orient(b,m);
    for(int i=0;i<n;++i) for(int j=0;j<m;++j) {
        ca[i][j]=area(a[i],a[(i+1)%n],b[j]);
        cb[i][j]=area(b[j],b[(j+1)%m],a[i]);
    }
    double ans=1e100;
    for(int s=0;s<m;++s) {
        for(int i=0;i<=n;++i) for(int j=0;j<=m;++j) dp[i][j]=1e100;
        dp[0][0]=0;
        for(int i=0;i<=n;++i) for(int j=0;j<=m;++j) {
            if(i<n) dp[i+1][j]=min(dp[i+1][j],dp[i][j]+ca[i][(s+j)%m]);
            if(j<m) dp[i][j+1]=min(dp[i][j+1],dp[i][j]+cb[i%n][(s+j)%m]);
        }
        ans=min(ans,dp[n][m]);
    }
    printf("%.0f\n",ans);
    return 0;
}
