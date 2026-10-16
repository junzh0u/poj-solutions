// POJ 4010 - Shadow
// Model: gpt-6.1-sol
// Project onto a plane perpendicular to the light: shields become circles.
// Integrate exposed circle arcs and convex-hull edges, then scale to ground.
// Coincident and contained circles are removed before boundary integration.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), EPS=1e-9;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} bool operator<(P b)const{return x<b.x||(x==b.x&&y<b.y);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;} double dot(P a,P b){return a.x*b.x+a.y*b.y;} double len(P a){return sqrt(dot(a,a));}
struct C {P p; double r;};
typedef pair<double,double> I;
vector<P> hull(vector<P> p){sort(p.begin(),p.end()); vector<P> h; int n=p.size(); for(int i=0;i<n;i++){while(h.size()>1&&cross(h.back()-h[h.size()-2],p[i]-h.back())<=EPS)h.pop_back(); h.push_back(p[i]);} int k=h.size(); for(int i=n-2;i>=0;i--){while((int)h.size()>k&&cross(h.back()-h[h.size()-2],p[i]-h.back())<=EPS)h.pop_back();h.push_back(p[i]);} if(h.size()>1)h.pop_back(); return h;}
void interval(vector<I>&v,double a,double b){while(a<0){a+=2*PI;b+=2*PI;} while(a>=2*PI){a-=2*PI;b-=2*PI;} if(b>2*PI){v.push_back(I(a,2*PI));v.push_back(I(0,b-2*PI));}else v.push_back(I(a,b));}
vector<I> gaps(vector<I> v,double end){sort(v.begin(),v.end()); vector<I> g; double t=0;for(size_t i=0;i<v.size();i++){double a=max(0.0,v[i].first),b=min(end,v[i].second);if(a>t)g.push_back(I(t,a));t=max(t,b);}if(t<end)g.push_back(I(t,end));return g;}
bool inside(P p,const vector<P>&h){if(h.size()<3)return false;for(size_t i=0;i<h.size();i++)if(cross(h[(i+1)%h.size()]-h[i],p-h[i])<-EPS)return false;return true;}
int main(){int n,m;double dx,dy,dz;while(scanf("%d%d%lf%lf%lf",&n,&m,&dx,&dy,&dz)==5&&(n||m)){double horiz=sqrt(dx*dx+dy*dy),scale=-dz/sqrt(dx*dx+dy*dy+dz*dz);P u(1,0),v(0,1);if(horiz>0){u=P(-dy/horiz,dx/horiz);v=P(dx/horiz,dy/horiz);} vector<C> c;vector<P> pts;for(int i=0;i<n+m;i++){double x,y,z,r=0;scanf("%lf%lf%lf",&x,&y,&z);if(i<n)scanf("%lf",&r);P q(x-z*dx/dz,y-z*dy/dz),p(dot(q,u),dot(q,v)*scale);if(i<n){C t;t.p=p;t.r=r;c.push_back(t);}else pts.push_back(p);}vector<P> h=hull(pts); vector<C> a;for(int i=0;i<n;i++){bool covered=false;for(int j=0;j<n;j++)if(i!=j&&len(c[i].p-c[j].p)+c[i].r<=c[j].r+EPS&&(c[j].r>c[i].r+EPS||j<i)){covered=true;break;}if(!covered)a.push_back(c[i]);}double area=0;
for(size_t i=0;i<a.size();i++){C t=a[i];vector<I> cov;for(size_t j=0;j<a.size();j++)if(i!=j){P d=a[j].p-t.p;double dis=len(d);if(dis>=t.r+a[j].r-EPS||dis<=fabs(t.r-a[j].r)+EPS)continue;double ang=atan2(d.y,d.x),w=acos(max(-1.0,min(1.0,(dis*dis+t.r*t.r-a[j].r*a[j].r)/(2*dis*t.r))));interval(cov,ang-w,ang+w);}vector<double> cuts;cuts.push_back(0);cuts.push_back(2*PI);for(size_t j=0;j<h.size();j++){P p=h[j]-t.p,d=h[(j+1)%h.size()]-h[j];double A=dot(d,d),B=2*dot(p,d),D=B*B-4*A*(dot(p,p)-t.r*t.r);if(A<EPS||D<0)continue;double root=sqrt(max(0.0,D));for(int s=-1;s<=1;s+=2){double z=(-B+s*root)/(2*A);if(z>=0&&z<=1){P q=p+d*z;double ang=atan2(q.y,q.x);if(ang<0)ang+=2*PI;cuts.push_back(ang);}}}sort(cuts.begin(),cuts.end());for(size_t j=1;j<cuts.size();j++){double mid=(cuts[j-1]+cuts[j])/2;if(inside(t.p+P(cos(mid),sin(mid))*t.r,h))cov.push_back(I(cuts[j-1],cuts[j]));}vector<I> g=gaps(cov,2*PI);for(size_t j=0;j<g.size();j++){double l=g[j].first,r=g[j].second;area+=(t.r*t.r*(r-l)+t.r*t.p.x*(sin(r)-sin(l))+t.r*t.p.y*(cos(l)-cos(r)))/2;}}
if(h.size()>=3)for(size_t i=0;i<h.size();i++){P p=h[i],d=h[(i+1)%h.size()]-p;double A=dot(d,d);vector<I>cov;for(size_t j=0;j<a.size();j++){P q=p-a[j].p;double B=2*dot(q,d),D=B*B-4*A*(dot(q,q)-a[j].r*a[j].r);if(D>0){double r=sqrt(D);double l=max(0.0,(-B-r)/(2*A)),u=min(1.0,(-B+r)/(2*A));if(l<u)cov.push_back(I(l,u));}}vector<I>g=gaps(cov,1);for(size_t j=0;j<g.size();j++)area+=cross(p,d)*(g[j].second-g[j].first)/2;}
printf("%.4f\n",max(0.0,area/scale));}return 0;}
