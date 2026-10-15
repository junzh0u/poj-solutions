// POJ 2298 - Great Circle
// Model: gpt-6.1-sol
// Parameterize the shorter spherical arc in an orthonormal basis and
// maximize its vertical coordinate at endpoints and its stationary point.
// Antipodal cities have no unique answer, except the two poles: every
// route contains the north pole. Coincident cities give their own latitude.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
int readangle(){int d,m;char c;scanf(" %d,%d%c",&d,&m,&c);int v=d*60+m;return (c=='S'||c=='W')?-v:v;}
struct V {double x,y,z;};
V point(int lat,int lon){double a=lat*PI/10800,b=lon*PI/10800;V p={cos(a)*cos(b),cos(a)*sin(b),sin(a)};return p;}
int main(){int n;scanf("%d",&n);while(n--){
 int a=readangle(),b=readangle(),c=readangle(),d=readangle();
 if(a==-c && abs(a)!=5400 && abs(b-d)==10800){puts("undefined");continue;}
 double ans;
 if(abs(a)==5400 || abs(c)==5400) ans=max(a,c)*PI/10800;
 else {
 V p=point(a,b),q=point(c,d);
 double dot=p.x*q.x+p.y*q.y+p.z*q.z;
 V cross={p.y*q.z-p.z*q.y,p.z*q.x-p.x*q.z,p.x*q.y-p.y*q.x};
 double s=sqrt(cross.x*cross.x+cross.y*cross.y+cross.z*cross.z);
 double theta=atan2(s,dot),best=max(p.z,q.z);
 if(s>1e-14){double u=(q.z-dot*p.z)/s;double t=atan2(u,p.z);if(t<0)t+=2*PI;
 if(t<=theta+1e-12)best=max(best,p.z*cos(t)+u*sin(t));}
 best=max(-1.0,min(1.0,best));ans=asin(best);
 }
 int m=(int)floor(fabs(ans)*10800/PI+0.5);printf("%d,%d%c\n",m/60,m%60,ans < -1e-12?'S':'N');
 }return 0;}
