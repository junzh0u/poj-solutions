// POJ 3151 - Driving Directions
// Model: gpt-6-astra
// Expand each rectangle by the saucer radius. Its boundary consists of
// straight sides and quarter circles. Build all unobstructed common tangents
// and endpoint tangents, then join consecutive points by unobstructed arcs.
// Split circles at every possible expanded-boundary intersection so a midpoint
// certifies each arc. Touching is allowed, including zero-width passages.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
using namespace std;
const double PI=acos(-1.0), EPS=1e-8;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double k)const{return P(x*k,y*k);} };
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double norm(P a){return sqrt(dot(a,a));}
struct Rect {double x1,y1,x2,y2;};
struct Circle {P p; double r; vector<pair<double,int> > a; Circle(P q,double R):p(q),r(R){} };
vector<Rect> rs; vector<Circle> cs;
vector<vector<pair<int,double> > > g;
double rad;
int node(){g.push_back(vector<pair<int,double> >());return (int)g.size()-1;}
void edge(int a,int b,double w){g[a].push_back(make_pair(b,w));g[b].push_back(make_pair(a,w));}
double angle(P p){double a=atan2(p.y,p.x);return a<0?a+2*PI:a;}
int add(int c,double a){while(a<0)a+=2*PI;while(a>=2*PI)a-=2*PI;int v=node();cs[c].a.push_back(make_pair(a,v));return v;}
double pd(P p,Rect t){double x=max(max(t.x1-p.x,p.x-t.x2),0.0), y=max(max(t.y1-p.y,p.y-t.y2),0.0);return x*x+y*y;}
bool valid(P p){for(size_t i=0;i<rs.size();++i)if(pd(p,rs[i])<rad*rad-EPS)return false;return true;}
double ps(P p,P a,P b){P v=b-a;double d=dot(v,v);double t=d>1e-20?max(0.0,min(1.0,dot(p-a,v)/d)):0;P q=p-(a+v*t);return dot(q,q);}
bool visible(P a,P b){
 for(size_t i=0;i<rs.size();++i){Rect t=rs[i];double lo=0,hi=1;P v=b-a;
  if(fabs(v.x)<1e-14){if(a.x<t.x1||a.x>t.x2)lo=2;}else{double l=(t.x1-a.x)/v.x,h=(t.x2-a.x)/v.x;if(l>h)swap(l,h);lo=max(lo,l);hi=min(hi,h);}
  if(fabs(v.y)<1e-14){if(a.y<t.y1||a.y>t.y2)lo=2;}else{double l=(t.y1-a.y)/v.y,h=(t.y2-a.y)/v.y;if(l>h)swap(l,h);lo=max(lo,l);hi=min(hi,h);}
  if(lo<=hi)return false;
  double d=min(pd(a,t),pd(b,t));
  d=min(d,ps(P(t.x1,t.y1),a,b));d=min(d,ps(P(t.x1,t.y2),a,b));
  d=min(d,ps(P(t.x2,t.y1),a,b));d=min(d,ps(P(t.x2,t.y2),a,b));
  if(d<rad*rad-EPS)return false;
 }return true;
}
int endpoint(int c,P p){return c<2?c:add(c,angle(p-cs[c].p));}
void tangents(int i,int j){
 P d=cs[j].p-cs[i].p;double z=dot(d,d);if(z<1e-20)return;
 for(int s=-1;s<=1;s+=2){double k=cs[i].r-s*cs[j].r;double h=z-k*k;if(h<-EPS)continue;h=sqrt(max(0.0,h));
  for(int side=-1;side<=1;side+=2){P u=(d*k+P(-d.y,d.x)*(h*side))*(1/z);P a=cs[i].p+u*cs[i].r,b=cs[j].p+u*(s*cs[j].r);
   if(visible(a,b))edge(endpoint(i,a),endpoint(j,b),norm(a-b));
  }
 }
}
int main(){int n;double ax,ay,bx,by;if(scanf("%lf%d",&rad,&n)!=2)return 0;scanf("%lf%lf%lf%lf",&ax,&ay,&bx,&by);
 cs.push_back(Circle(P(ax,ay),0));cs.push_back(Circle(P(bx,by),0));node();node();
 for(int i=0;i<n;++i){Rect t;scanf("%lf%lf%lf%lf",&t.x1,&t.y1,&t.x2,&t.y2);rs.push_back(t);cs.push_back(Circle(P(t.x1,t.y1),rad));cs.push_back(Circle(P(t.x1,t.y2),rad));cs.push_back(Circle(P(t.x2,t.y1),rad));cs.push_back(Circle(P(t.x2,t.y2),rad));}
 if(visible(cs[0].p,cs[1].p))edge(0,1,norm(cs[0].p-cs[1].p));
 for(int i=0;i<(int)cs.size();++i)for(int j=i+1;j<(int)cs.size();++j)tangents(i,j);
 for(int i=2;i<(int)cs.size();++i){
  for(int k=0;k<4;++k)add(i,k*PI/2);
  // Crossing an offset side line or a corner circle includes every place
  // at which this circle can enter or leave an expanded rectangle.
  for(int j=0;j<n;++j){double xs[4]={rs[j].x1-rad,rs[j].x1+rad,rs[j].x2-rad,rs[j].x2+rad};double ys[4]={rs[j].y1-rad,rs[j].y1+rad,rs[j].y2-rad,rs[j].y2+rad};
   for(int k=0;k<4;++k){double q=(xs[k]-cs[i].p.x)/rad;if(fabs(q)<=1){double a=acos(q);add(i,a);add(i,-a);}q=(ys[k]-cs[i].p.y)/rad;if(fabs(q)<=1){double a=asin(q);add(i,a);add(i,PI-a);}}
  }
  for(int j=i+1;j<(int)cs.size();++j){P d=cs[j].p-cs[i].p;double len=norm(d);if(len>2*rad+EPS||len<EPS)continue;double a=angle(d),b=acos(min(1.0,len/(2*rad)));
   for(int s=-1;s<=1;s+=2){double t=a+s*b;P p=cs[i].p+P(cos(t),sin(t))*rad;int u=add(i,t),v=add(j,angle(p-cs[j].p));if(valid(p))edge(u,v,0);}
  }
 }
 for(int i=2;i<(int)cs.size();++i){vector<pair<double,int> >& a=cs[i].a;sort(a.begin(),a.end());for(size_t k=0;k<a.size();++k){size_t l=(k+1)%a.size();double t=a[l].first+(l==0?2*PI:0),mid=(a[k].first+t)/2;P p=cs[i].p+P(cos(mid),sin(mid))*rad;if(valid(p))edge(a[k].second,a[l].second,(t-a[k].first)*rad);}}
 vector<double> ds(g.size(),1e100);ds[0]=0;typedef pair<double,int> Item;priority_queue<Item,vector<Item>,greater<Item> > q;q.push(make_pair(0.0,0));
 while(!q.empty()){Item it=q.top();q.pop();int u=it.second;if(it.first>ds[u]+1e-10)continue;if(u==1)break;for(size_t k=0;k<g[u].size();++k){int v=g[u][k].first;double d=ds[u]+g[u][k].second;if(d<ds[v]){ds[v]=d;q.push(make_pair(d,v));}}}
 if(ds[1]>1e90)puts("no solution");else printf("%.10f\n",ds[1]);return 0;
}
