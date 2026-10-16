// POJ 3457 - Mineshaft
// Model: gpt-6.1-sol
// Enumerate lines through two wall vertices and their connected portions in
// the shaft. A breadth-first search retains the earliest reachable entry on
// each directed portion; two wall contacts must lie strictly between bends.
// Collinear wall intervals supply arbitrarily many distinct attachment points.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
const double E=1e-8, INF=1e100;
struct P {double x,y; P(double X=0,double Y=0):x(X),y(Y){}};
struct I {double a,b; I(double A=0,double B=0):a(A),b(B){}};
struct L {double m,c,a,b; vector<I> touch;};
struct Edge {int j; double y,gate[2]; bool eq[2]; Edge(int J,double Y):j(J),y(Y){}};
vector<P> w[2]; vector<L> ls;
double xx(int k,double y) {for(unsigned i=1;i<w[k].size();++i) if(y<=w[k][i].y+E) {P a=w[k][i-1],b=w[k][i];return a.x+(b.x-a.x)*(y-a.y)/(b.y-a.y);}return w[k].back().x;}
bool supported(const L& l,double a,double b,bool incA,bool incB) {
 if(a>b) {swap(a,b);swap(incA,incB);} if(b-a<E)return false;
 double first=INF,last=-INF;
 for(unsigned i=0;i<l.touch.size();++i) {
  double x=max(a,l.touch[i].a),y=min(b,l.touch[i].b);
  if(x>y+E)continue;
  if(y-x>E)return true;
  if((x>a+E||incA)&&(x<b-E||incB)){first=min(first,x);last=max(last,x);}
 }
 return last-first>E;
}
// For an exit, find the second-last attachment in traversal order.
void prepare(const L& l, Edge& e, int d) {
 double end=d?-e.y:e.y, first=-INF, second=-INF, interval=-INF;
 for(unsigned t=0;t<l.touch.size();++t) {
  double a=d?-l.touch[t].b:l.touch[t].a;
  double b=d?-l.touch[t].a:l.touch[t].b;
  if(b-a>E) {if(a<end-E)interval=max(interval,min(b,end));}
  else if(a<end-E) {if(a>first+E){second=first;first=a;}else if(a<first-E)second=max(second,a);}
 }
 e.gate[d]=max(interval,second);e.eq[d]=(second>interval+E);
}
int main(){int T;cin>>T;while(T--){
 vector<P> p;for(int k=0;k<2;++k){int n;cin>>n;w[k].clear();while(n--){double x,y;cin>>x>>y;w[k].push_back(P(x,y));p.push_back(P(x,y));}}
 double bot=w[0][0].y,top=w[0].back().y;ls.clear();vector<P> lines;
 for(unsigned i=0;i<p.size();++i)for(unsigned j=0;j<i;++j){if(fabs(p[i].y-p[j].y)<E)continue;double m=(p[i].x-p[j].x)/(p[i].y-p[j].y),c=p[i].x-m*p[i].y;bool dup=false;for(unsigned z=0;z<lines.size();++z)if(fabs(lines[z].x-m)<E&&fabs(lines[z].y-c)<E){dup=true;break;}if(!dup)lines.push_back(P(m,c));}
 for(unsigned z=0;z<lines.size();++z){double m=lines[z].x,c=lines[z].y;vector<double> cuts;vector<I> ct;cuts.push_back(bot);cuts.push_back(top);
 for(int k=0;k<2;++k)for(unsigned i=1;i<w[k].size();++i){P a=w[k][i-1],b=w[k][i];cuts.push_back(a.y);cuts.push_back(b.y);double da=a.x-m*a.y-c,db=b.x-m*b.y-c;
 if(fabs(da)<E&&fabs(db)<E)ct.push_back(I(a.y,b.y));else if(da*db<=0||fabs(da)<E||fabs(db)<E){double y=a.y+(b.y-a.y)*da/(da-db);if(y>=a.y-E&&y<=b.y+E){cuts.push_back(y);ct.push_back(I(y,y));}}}
 sort(cuts.begin(),cuts.end());vector<double> v;for(unsigned i=0;i<cuts.size();++i)if(v.empty()||cuts[i]-v.back()>E)v.push_back(cuts[i]);
 for(unsigned i=0;i+1<v.size();){double y=(v[i]+v[i+1])/2,x=m*y+c;if(x<xx(0,y)-E||x>xx(1,y)+E){++i;continue;}unsigned j=i+1;while(j+1<v.size()){y=(v[j]+v[j+1])/2;x=m*y+c;if(x<xx(0,y)-E||x>xx(1,y)+E)break;++j;}
 L l;l.m=m;l.c=c;l.a=v[i];l.b=v[j];for(unsigned t=0;t<ct.size();++t)if(ct[t].b>=l.a-E&&ct[t].a<=l.b+E)l.touch.push_back(I(max(l.a,ct[t].a),min(l.b,ct[t].b)));
 if(supported(l,l.a,l.b,fabs(l.a-bot)<E,fabs(l.b-top)<E))ls.push_back(l);i=j;}
 }
 int n=ls.size();vector<vector<Edge> > adj(n);for(int i=0;i<n;++i)for(int j=0;j<i;++j)if(fabs(ls[i].m-ls[j].m)>E){double y=(ls[j].c-ls[i].c)/(ls[i].m-ls[j].m);if(y>=max(ls[i].a,ls[j].a)-E&&y<=min(ls[i].b,ls[j].b)+E){adj[i].push_back(Edge(j,y));adj[j].push_back(Edge(i,y));}}
 for(int i=0;i<n;++i)for(unsigned e=0;e<adj[i].size();++e)for(int d=0;d<2;++d)prepare(ls[i],adj[i][e],d);
 vector<double> reach(2*n,INF);for(int i=0;i<n;++i)if(fabs(ls[i].a-bot)<E)reach[2*i]=bot;
 int ans=-1;for(int bends=0;bends<=4*n;++bends){vector<double> next=reach;bool changed=false;
 for(int i=0;i<n;++i)for(int d=0;d<2;++d){int s=2*i+d;double ent=reach[s];if(ent==INF)continue;double from=d?-ent:ent;
 if((!d&&top>=from-E)||(d&&top<=from+E))if(top>=ls[i].a-E&&top<=ls[i].b+E&&supported(ls[i],from,top,fabs(from-bot)<E,true)){ans=bends;break;}
 for(unsigned e=0;e<adj[i].size();++e){double y=adj[i][e].y;if((d?-y:y)<=ent+E)continue;if(!(ent<adj[i][e].gate[d]-E || (adj[i][e].eq[d]&&fabs(ent-adj[i][e].gate[d])<E&&fabs(from-bot)<E)))continue;int j=adj[i][e].j;for(int k=0;k<2;++k){double val=k?-y:y;if(val<next[2*j+k]-E){next[2*j+k]=val;changed=true;}}}
 }
 if(ans>=0)break;
 if(!changed)break;
 reach.swap(next);
 }
 cout<<ans<<'\n';}}
