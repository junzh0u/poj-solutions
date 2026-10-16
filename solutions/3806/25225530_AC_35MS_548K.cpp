// POJ 3806 - Origami Through-Hole
// Model: gpt-6.1-sol
// Split convex paper pieces, propagate moving hinges and upper overlaps,
// then reflect the selected stack in reverse order. Original coordinates
// distinguish physical hinges from edges that merely coincide after folding.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double E=1e-7;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} };
double dot(P a,P b){return a.x*b.x+a.y*b.y;} double cross(P a,P b){return a.x*b.y-a.y*b.x;}
struct V { P p,o; V(P a=P(),P b=P()):p(a),o(b){} };
typedef vector<V> Poly;
Poly clip(const Poly&a,P n,double c,int side){Poly b; for(unsigned i=0;i<a.size();++i){V u=a[i],v=a[(i+1)%a.size()];double x=(dot(u.p,n)-c)*side,y=(dot(v.p,n)-c)*side;if(x>=-E)b.push_back(u);if((x>E&&y< -E)||(x< -E&&y>E)){double t=x/(x-y);b.push_back(V(u.p+(v.p-u.p)*t,u.o+(v.o-u.o)*t));}}return b;}
double area(const Poly&a){double s=0;for(unsigned i=0;i<a.size();++i)s+=cross(a[i].p,a[(i+1)%a.size()].p);return s;}
bool contains(const Poly&a,P p){double s=area(a)>0?1:-1;for(unsigned i=0;i<a.size();++i)if(s*cross(a[(i+1)%a.size()].p-a[i].p,p-a[i].p)<-E)return false;return true;}
bool separated(const Poly&a,const Poly&b){double s=area(a)>0?1:-1;for(unsigned i=0;i<a.size();++i){double best=-1e100;P u=a[i].p,v=a[(i+1)%a.size()].p;for(unsigned j=0;j<b.size();++j)best=max(best,s*cross(v-u,b[j].p-u));if(best<E)return true;}return false;}
bool overlap(const Poly&a,const Poly&b){return !separated(a,b)&&!separated(b,a);}
bool hinge(const Poly&a,const Poly&b){for(unsigned i=0;i<a.size();++i){P u=a[i].o,v=a[(i+1)%a.size()].o,d=v-u;double len=dot(d,d);if(len<E)continue;for(unsigned j=0;j<b.size();++j){P w=b[j].o,z=b[(j+1)%b.size()].o;if(fabs(cross(d,w-u))>E||fabs(cross(d,z-u))>E)continue;double l=dot(w-u,d)/len,r=dot(z-u,d)/len;if(l>r)swap(l,r);if(min(1.0,r)-max(0.0,l)>E)return true;}}return false;}
int main(){int k;while(scanf("%d",&k)==1&&k){vector<Poly>a(1);P corners[4]={P(0,0),P(100,0),P(100,100),P(0,100)};for(int i=0;i<4;++i)a[0].push_back(V(corners[i],corners[i]));while(k--){P p,q;scanf("%lf%lf%lf%lf",&p.x,&p.y,&q.x,&q.y);P n=p-q;double len=sqrt(dot(n,n));n=n*(1/len);double c=dot((p+q)*.5,n);vector<Poly>b;vector<int>side;for(unsigned i=0;i<a.size();++i)for(int s=-1;s<=1;s+=2){Poly t=clip(a[i],n,c,s);if(t.size()>=3&&fabs(area(t))>E){b.push_back(t);side.push_back(s);}}int N=b.size(),start=-1;for(int i=N-1;i>=0;--i)if(side[i]==1&&contains(b[i],p)){start=i;break;}vector<int>move(N,0),queue;if(start>=0){move[start]=1;queue.push_back(start);}for(unsigned h=0;h<queue.size();++h){int i=queue[h];for(int j=0;j<N;++j)if(side[j]==1&&!move[j]&&(hinge(b[i],b[j])||(j>i&&overlap(b[i],b[j])))){move[j]=1;queue.push_back(j);}}a.clear();for(int i=0;i<N;++i)if(!move[i])a.push_back(b[i]);for(int i=N-1;i>=0;--i)if(move[i]){for(unsigned j=0;j<b[i].size();++j){P &v=b[i][j].p;v=v-n*(2*(dot(v,n)-c));}a.push_back(b[i]);}}
P h;scanf("%lf%lf",&h.x,&h.y);int ans=0;for(unsigned i=0;i<a.size();++i)if(contains(a[i],h))++ans;printf("%d\n",ans);}return 0;}
