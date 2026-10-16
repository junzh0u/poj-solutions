// POJ 1536 - Trains
// Model: gpt-6.1-sol
// Propagate earliest reachable times through periodic safe gaps at crossings.
// Train occupancy includes its head arrival but excludes its rear departure;
// a car may wait at a crossing only strictly before the next head arrival.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double EPS=1e-9, INF=1e100;
struct Point { double x,y; };
struct Gap { double a,b; Gap(double x,double y):a(x),b(y){} };
struct Cross { double y,s; };
bool cmpcross(const Cross&a,const Cross&b){return a.y<b.y;}
double dist(Point a,Point b){return hypot(a.x-b.x,a.y-b.y);}
double mod(double a,double p){a=fmod(a,p);if(a<0)a+=p;return a;}
bool cmpgap(const Gap&a,const Gap&b){return a.a<b.a;}
int main(){
 int n,m;double ex,ey,vt,vc;
 while(scanf("%d%d%lf%lf%lf%lf",&n,&m,&ex,&ey,&vt,&vc)==6 && n){
  vector<Point> p(n+1);vector<double> cum(n+1,0);
  for(int i=0;i<n;i++)scanf("%lf%lf",&p[i].x,&p[i].y);
  p[n]=p[0];for(int i=0;i<n;i++)cum[i+1]=cum[i]+dist(p[i],p[i+1]);
  double perimeter=cum[n],period=perimeter/vt;
  vector<double> h(m),len(m);
  for(int j=0;j<m;j++){
   Point q;scanf("%lf%lf%lf",&q.x,&q.y,&len[j]);h[j]=0;
   for(int i=0;i<n;i++)if(fabs(dist(p[i],q)+dist(q,p[i+1])-dist(p[i],p[i+1]))<1e-7){h[j]=cum[i]+dist(p[i],q);break;}
  }
  vector<Cross> c;
  for(int i=0;i<n;i++){
   Point a=p[i],b=p[i+1];
   if((a.x<=ex && b.x>ex)||(b.x<=ex && a.x>ex)){
    double u=(ex-a.x)/(b.x-a.x),y=a.y+u*(b.y-a.y);
    if(y>=-EPS && y<=ey+EPS){Cross z;z.y=y;z.s=cum[i]+u*dist(a,b);c.push_back(z);}
   }
  }
  sort(c.begin(),c.end(),cmpcross);
  vector<Gap> prev;prev.push_back(Gap(0,INF));vector<double> best(1,0);
  double py=0;bool impossible=false;
  for(unsigned k=0;k<c.size();k++){
   vector<Gap> blocked;
   for(int j=0;j<m;j++){
    double a=mod(c[k].s-h[j],perimeter)/vt,b=a+len[j]/vt;
    if(b-a>=period-EPS){blocked.clear();blocked.push_back(Gap(0,period));break;}
    if(b<=period)blocked.push_back(Gap(a,b));
    else {blocked.push_back(Gap(a,period));blocked.push_back(Gap(0,b-period));}
   }
   sort(blocked.begin(),blocked.end(),cmpgap);vector<Gap> merged;
   for(unsigned j=0;j<blocked.size();j++){
    if(merged.empty() || blocked[j].a>merged.back().b+EPS)merged.push_back(blocked[j]);
    else merged.back().b=max(merged.back().b,blocked[j].b);
   }
   vector<Gap> gaps;
   if(merged.empty())gaps.push_back(Gap(0,period));
   else {
    for(unsigned j=0;j+1<merged.size();j++)if(merged[j+1].a>merged[j].b+EPS)gaps.push_back(Gap(merged[j].b,merged[j+1].a));
    double a=merged.back().b,b=merged[0].a+period;
    if(b>a+EPS)gaps.push_back(Gap(a,b));
   }
   // Expand enough periods: an earliest feasible route can wait less than
   // one period at each stop, including the starting point.
   vector<Gap> absolute;
   double horizon=ey/vc+(c.size()+2)*period;
   if(merged.empty())absolute.push_back(Gap(0,INF));
   else for(int cycle=-1;cycle<=(int)(horizon/period)+1;cycle++)
    for(unsigned z=0;z<gaps.size();z++){
     double a=gaps[z].a+cycle*period,b=gaps[z].b+cycle*period;
     if(b>0 && a<=horizon)absolute.push_back(Gap(max(0.0,a),b));
    }
   sort(absolute.begin(),absolute.end(),cmpgap);gaps=absolute;
   vector<double> next(gaps.size(),INF);double travel=(c[k].y-py)/vc;
   unsigned pos=0;
   for(unsigned z=0;z<gaps.size();z++){
    while(pos<prev.size() && (best[pos]>=INF/2 || prev[pos].b+travel<=gaps[z].a+EPS))pos++;
    for(unsigned j=pos;j<prev.size();j++)if(best[j]<INF/2){
     double arrival=max(best[j]+travel,gaps[z].a);
     if(arrival>=gaps[z].b-EPS)break;
     if(arrival-travel<prev[j].b-EPS){next[z]=arrival;break;}
    }
   }
   prev=gaps;best=next;py=c[k].y;
  }
  double answer=INF;for(unsigned j=0;j<best.size();j++)answer=min(answer,best[j]+(ey-py)/vc);
  if(answer>=INF/2)impossible=true;
  if(impossible)puts("Impossible!");else printf("%.4f\n",answer+1e-10);
 }
}
