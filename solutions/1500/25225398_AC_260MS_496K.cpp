// POJ 1500 - Polygonal Puzzle
// Model: gpt-6.1-sol
// Fit every cyclic correspondence by a translated, scaled rotation, then
// minimize the total squared residual with a minimum-cost assignment.
// Coordinates have upward y; the reported angle undoes the camera rotation.
// For rotational symmetry choose the smallest clockwise alignment angle.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { double x,y; Point(double a=0,double b=0):x(a),y(b){} };
const double PI=acos(-1.0);
void center(vector<Point>& p) { double x=0,y=0; int n=p.size(); for(int i=0;i<n;++i){x+=p[i].x;y+=p[i].y;} x/=n;y/=n; for(int i=0;i<n;++i){p[i].x-=x;p[i].y-=y;} }
int main(){
 int n,m;
 while(scanf("%d%d",&n,&m)==2 && n>0 && m>0){
 vector<vector<Point> > a(n,vector<Point>(m)),b=a;
 vector<double> aa(n,0),bb(n,0);
 for(int i=0;i<n;++i){for(int j=0;j<m;++j){double r;scanf("%lf",&r);a[i][j]=Point(r*sin(2*PI*j/m),r*cos(2*PI*j/m));}center(a[i]);for(int j=0;j<m;++j)aa[i]+=a[i][j].x*a[i][j].x+a[i][j].y*a[i][j].y;}
 for(int i=0;i<n;++i){for(int j=0;j<m;++j)scanf("%lf%lf",&b[i][j].x,&b[i][j].y);center(b[i]);for(int j=0;j<m;++j)bb[i]+=b[i][j].x*b[i][j].x+b[i][j].y*b[i][j].y;}
 vector<vector<double> > cost(n,vector<double>(n,1e100)),angle=cost;
 for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int s=0;s<m;++s){
 double dot=0,cross=0;
 for(int k=0;k<m;++k){const Point &p=a[j][k],&q=b[i][(k+s)%m];dot+=p.x*q.x+p.y*q.y;cross+=p.x*q.y-p.y*q.x;}
 double err=1-(dot*dot+cross*cross)/(aa[j]*bb[i]);
 double ang=atan2(cross,dot)*180/PI;if(ang<0)ang+=360;if(ang>359.999999)ang=0;
 if(err<cost[i][j]-1e-12 || (fabs(err-cost[i][j])<1e-12 && ang<angle[i][j])){cost[i][j]=err;angle[i][j]=ang;}
 }
 vector<double> u(n+1),v(n+1);vector<int> p(n+1),way(n+1);
 for(int i=1;i<=n;++i){p[0]=i;int j0=0;vector<double> d(n+1,1e100);vector<int> used(n+1,0);do{used[j0]=1;int i0=p[j0],j1=0;double delta=1e100;for(int j=1;j<=n;++j)if(!used[j]){double cur=cost[i0-1][j-1]-u[i0]-v[j];if(cur<d[j]){d[j]=cur;way[j]=j0;}if(d[j]<delta){delta=d[j];j1=j;}}for(int j=0;j<=n;++j)if(used[j]){u[p[j]]+=delta;v[j]-=delta;}else d[j]-=delta;j0=j1;}while(p[j0]);do{int j1=way[j0];p[j0]=p[j1];j0=j1;}while(j0);}
 vector<int> match(n);for(int j=1;j<=n;++j)match[p[j]-1]=j-1;
 for(int i=0;i<n;++i){double ang=angle[i][match[i]];if(ang>=359.95)ang=0;printf("%d %.1f\n",match[i]+1,ang);}
 }
 return 0;
}
