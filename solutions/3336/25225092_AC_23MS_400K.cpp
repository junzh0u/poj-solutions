// POJ 3336 - ACM Underground
// Model: gpt-6.1-sol
// Split lines only at policemen lying on exactly one original line.
// Union intersecting safe pieces unless their sole transfer point is guarded.
// A policeman at an intersection forbids changing lines, not passing through.
// Dynamic storage accommodates the discussion's corrected n/m bounds.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct P { long double x,y; P(long double X=0,long double Y=0):x(X),y(Y){} };
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
long double cross(P a,P b){return a.x*b.y-a.y*b.x;}
long double dot(P a,P b){return a.x*b.x+a.y*b.y;}
struct L {P a,b; int id; bool oa,ob; L(P A,P B,int I,bool O=false,bool Q=false):a(A),b(B),id(I),oa(O),ob(Q){} };
bool on(P p,const L& l){return cross(l.b-l.a,p-l.a)==0 && dot(p-l.a,p-l.b)<=0;}
bool inside(P p,const L& l){return on(p,l) && !(l.oa && p.x==l.a.x && p.y==l.a.y) && !(l.ob && p.x==l.b.x && p.y==l.b.y);}
vector<int> par;
int root(int x){return par[x]==x?x:par[x]=root(par[x]);}
void join(int a,int b){par[root(a)]=root(b);}
struct Cut {long double t;P p;Cut(long double T,P Q):t(T),p(Q){} bool operator<(const Cut& b)const{return t<b.t;} };
int main(){int T;cin>>T;while(T--){int n,m;cin>>n>>m;P s,d;cin>>s.x>>s.y>>d.x>>d.y;vector<L> lines;for(int i=0;i<n;i++){P a,b;cin>>a.x>>a.y>>b.x>>b.y;lines.push_back(L(a,b,i));}vector<P> cops(m);vector<vector<Cut> > cuts(n);vector<vector<bool> > blocked(n,vector<bool>(n,false));
for(int k=0;k<m;k++){cin>>cops[k].x>>cops[k].y;vector<int> ids;for(int i=0;i<n;i++)if(on(cops[k],lines[i]))ids.push_back(i);if(ids.size()==1){int i=ids[0];cuts[i].push_back(Cut(dot(cops[k]-lines[i].a,lines[i].b-lines[i].a),cops[k]));}else for(unsigned i=0;i<ids.size();i++)for(unsigned j=0;j<ids.size();j++)blocked[ids[i]][ids[j]]=true;}
vector<L> pieces;for(int i=0;i<n;i++){sort(cuts[i].begin(),cuts[i].end());P prev=lines[i].a;bool open=false;for(unsigned k=0;k<cuts[i].size();k++){P q=cuts[i][k].p;if(q.x!=prev.x||q.y!=prev.y)pieces.push_back(L(prev,q,i,open,true));prev=q;open=true;}P q=lines[i].b;if(q.x!=prev.x||q.y!=prev.y||!open)pieces.push_back(L(prev,q,i,open,false));}
int z=pieces.size();par.resize(z+2);for(int i=0;i<z+2;i++)par[i]=i;for(int i=0;i<z;i++){if(inside(s,pieces[i]))join(i,z);if(inside(d,pieces[i]))join(i,z+1);for(int j=0;j<i;j++){L a=pieces[i],b=pieces[j];if(a.id==b.id)continue;P u=a.b-a.a,v=b.b-b.a;long double den=cross(u,v);if(den!=0){if(blocked[a.id][b.id])continue;long double x=cross(b.a-a.a,v)/den,y=cross(b.a-a.a,u)/den;if(x<0||x>1||y<0||y>1)continue;if((x==0&&a.oa)||(x==1&&a.ob)||(y==0&&b.oa)||(y==1&&b.ob))continue;join(i,j);}else if(cross(b.a-a.a,u)==0){vector<P> ps;ps.push_back(a.a);ps.push_back(a.b);ps.push_back(b.a);ps.push_back(b.b);bool ok=false;for(int k=0;k<4;k++)if(inside(ps[k],a)&&inside(ps[k],b)){bool cop=false;for(int h=0;h<m;h++)if(cops[h].x==ps[k].x&&cops[h].y==ps[k].y)cop=true;if(!cop)ok=true;}for(int k=0;k<4;k++)for(int h=0;h<k;h++){P mid((ps[k].x+ps[h].x)/2,(ps[k].y+ps[h].y)/2);if((ps[k].x!=ps[h].x||ps[k].y!=ps[h].y)&&inside(mid,a)&&inside(mid,b))ok=true;}if(ok)join(i,j);}}}cout<<(root(z)==root(z+1)?"YES":"NO")<<'\n';}}
