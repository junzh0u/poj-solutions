// POJ 4053 - World of Goo
// Model: gpt-6.1-sol
// Event simulation of a rigid construction: free fall, then clockwise rotations
// about successive ground contacts. Solve segment/exit contacts analytically.
// The discussion confirms falling does not alter the prescribed rotation speed.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), E=1e-8, INF=1e100;
struct P {double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double k)const{return P(x*k,y*k);} };
double cr(P a,P b){return a.x*b.y-a.y*b.x;} double dot(P a,P b){return a.x*b.x+a.y*b.y;} double norm(P a){return sqrt(dot(a,a));}
P rot(P a,double t){return P(a.x*cos(t)-a.y*sin(t),a.x*sin(t)+a.y*cos(t));}
P p[55]; double v[55]; int u[205],w[205],n,m;
bool on(P q,P a,P b){P d=b-a;return fabs(cr(d,q-a))<=E*(1+norm(d)) && dot(q-a,q-b)<=E;}
double contact(P q,int pivot,double lim){double ans=INF;for(int k=0;k<m;k++){P a=p[u[k]]-p[pivot],b=p[w[k]]-p[pivot],d=b-a; if(on(q,a,b))return 0;double A=cr(d,q),B=dot(d,q),C=cr(d,a),R=hypot(A,B); if(R<E)continue;double z=C/R;if(z>1+E||z<-1-E)continue;z=max(-1.0,min(1.0,z));double phi=atan2(B,A),alpha=acos(z);for(int s=-1;s<=1;s+=2){double t=phi+s*alpha; t=fmod(t,2*PI);if(t<0)t+=2*PI;if(t<=lim+E && on(rot(q,t),a,b))ans=min(ans,t);}}return ans;}
int main(){while(scanf("%d%d",&n,&m)==2&&(n||m)){double low=INF;for(int i=0;i<n;i++){scanf("%lf%lf%lf",&p[i].x,&p[i].y,&v[i]);low=min(low,p[i].y);}for(int i=0;i<m;i++){double z;scanf("%d%d%lf",&u[i],&w[i],&z);v[u[i]]/=z;v[w[i]]/=z;}P q;scanf("%lf%lf",&q.x,&q.y);double hit=INF;
for(int k=0;k<m;k++){P a=p[u[k]],b=p[w[k]],d=b-a; if(fabs(d.x)>E){double t=(q.x-a.x)/d.x;if(t>=-E&&t<=1+E){double fall=a.y+d.y*t-q.y;if(fall>=-E&&fall<=low+E)hit=min(hit,sqrt(max(0.0,2*fall/9.8)));}}else if(fabs(q.x-a.x)<E){double fall=max(0.0,min(a.y,b.y)-q.y);if(q.y<=max(a.y,b.y)+E&&fall<=low+E)hit=min(hit,sqrt(max(0.0,2*fall/9.8)));}}
if(hit<INF){printf("%.2f\n",hit);continue;}double tm=sqrt(2*low/9.8);for(int i=0;i<n;i++)p[i].y-=low;int pivot=0;for(int i=1;i<n;i++)if(p[i].y<p[pivot].y-E||(fabs(p[i].y-p[pivot].y)<E&&p[i].x>p[pivot].x))pivot=i;
double turned=0,startx=p[pivot].x,starttime=tm;bool skipped=false;int anchor=-1;double answer=INF;
for(int it=0;it<10000;it++){double minx=INF,maxx=-INF;for(int i=0;i<n;i++){minx=min(minx,p[i].x);maxx=max(maxx,p[i].x);}if(q.x<minx-E||q.y<-E)break;double angle=2*PI;int next=pivot;for(int i=0;i<n;i++)if(norm(p[i]-p[pivot])>E){double a=atan2(max(0.0,p[i].y),p[i].x-p[pivot].x);if(a<angle){angle=a;next=i;}}
double h=contact(q-p[pivot],pivot,angle);if(h<E){answer=tm;break;}if(angle>=2*PI-E||v[pivot]<=0)break;if(h<INF){answer=tm+h/(2*PI*v[pivot]);break;}tm+=angle/(2*PI*v[pivot]);P center=p[pivot];for(int i=0;i<n;i++)p[i]=center+rot(p[i]-center,-angle);p[next].y=0;pivot=next;turned+=angle;
if(anchor<0){anchor=pivot;turned=0;startx=p[pivot].x;starttime=tm;}else if(!skipped&&pivot==anchor&&turned>=2*PI-E){double shift=p[pivot].x-startx;double mx=-INF;for(int i=0;i<n;i++)mx=max(mx,p[i].x);if(shift>E){double cycles=floor((q.x-mx)/shift);if(cycles>0){double dx=cycles*shift;for(int i=0;i<n;i++)p[i].x+=dx;tm+=cycles*(tm-starttime);}}skipped=true;}
}if(answer<INF)printf("%.2f\n",answer);else puts("Bad goo!");}return 0;}
