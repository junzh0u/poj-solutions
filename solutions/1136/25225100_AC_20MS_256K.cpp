// POJ 1136 - Pendulum
// Model: gpt-6.1-sol
// Follow counterclockwise circular arcs, choosing the first reachable hook.
// Reaching height zero reverses and retraces every arc. Once an entire
// circle lies below zero, only the eventual final circle is periodic.
// Repeated directed hook states allow whole winding cycles to be skipped.
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), EPS=1e-10;
struct P {double x,y;};
struct Step {double r,d;};
double norm(double a) {while(a<0)a+=2*PI;while(a>=2*PI)a-=2*PI;return a;}
double solve(vector<P>& p,double r) {
 int cur=0,prev=-1; double angle=PI, sum=0; bool trapped=false;
 map<pair<int,int>,int> seen; vector<Step> hist;
 for (;;) {
  if(r < -p[cur].y-EPS) trapped=true;
  pair<int,int> key(prev,cur);
  if(trapped && seen.count(key)) {
   int start=seen[key]; double cycle=hist[start].r-r, slack=1e300;
   for(int k=start;k<(int)hist.size();++k) slack=min(slack,hist[k].r-hist[k].d-cycle);
   if(cycle>EPS && slack>cycle) {double times=floor((slack-EPS)/cycle); r-=times*cycle;}
   seen.clear();hist.clear();
  }
  if(trapped) seen[key]=(int)hist.size();
  double limit=2*PI;
  if(!trapped) {double end=asin(max(-1.0,min(1.0,-p[cur].y/r))); limit=norm(end-angle);if(limit<EPS)limit=2*PI;}
  int next=-1;double turn=limit,dist=0;
  for(int i=1;i<(int)p.size();++i) if(i!=cur) {
   double dx=p[i].x-p[cur].x,dy=p[i].y-p[cur].y,d=sqrt(dx*dx+dy*dy);
   if(d<EPS || d>=r-EPS)continue;
   double t=norm(atan2(dy,dx)-angle);
   if(t<turn-EPS || (fabs(t-turn)<EPS && (next>=0 && d<dist))) {turn=t;next=i;dist=d;}
  }
  if(next<0) return trapped ? 2*PI*r : 2*(sum+r*limit);
  if(trapped) {Step s={r,dist};hist.push_back(s);} else sum+=r*turn;
  angle=atan2(p[next].y-p[cur].y,p[next].x-p[cur].x);
  r-=dist;prev=cur;cur=next;
 }
}
int main(){int n,tc=0;double r;while(scanf("%d%lf",&n,&r)==2 && r!=0){vector<P> p(n+1);p[0].x=p[0].y=0;for(int i=1;i<=n;++i)scanf("%lf%lf",&p[i].x,&p[i].y);printf("Pendulum #%d\nLength of periodic orbit = %.2f\n\n",++tc,solve(p,r));}return 0;}
