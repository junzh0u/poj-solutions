// POJ 3397 - Click the Lines
// Model: gpt-6-astra
// Enumerate intersections of the distance-strip boundaries, then solve
// minimum set cover by exact branch and bound with packing lower bounds.
// Clicks are arbitrary points as stated: the second published sample is
// inconsistent, since (-0.75,-0.75) selects all its lines with D=1.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;
const int W=7;
struct Bits {
 unsigned int x[W];
 Bits(){for(int i=0;i<W;i++)x[i]=0;}
 void add(int i){x[i/32]|=1U<<(i%32);}
 bool has(int i)const{return (x[i/32]>>(i%32))&1U;}
 bool operator<(const Bits& b)const{for(int i=0;i<W;i++)if(x[i]!=b.x[i])return x[i]<b.x[i];return false;}
};
int pc(unsigned int x){x=x-((x>>1)&0x55555555U);x=(x&0x33333333U)+((x>>2)&0x33333333U);return (((x+(x>>4))&0x0f0f0f0fU)*0x01010101U)>>24;}
int count(const Bits&a){int r=0;for(int i=0;i<W;i++)r+=pc(a.x[i]);return r;}
Bits inter(const Bits&a,const Bits&b){Bits c;for(int i=0;i<W;i++)c.x[i]=a.x[i]&b.x[i];return c;}
Bits minusB(const Bits&a,const Bits&b){Bits c;for(int i=0;i<W;i++)c.x[i]=a.x[i]&~b.x[i];return c;}
struct Line{double a,b,c;};
vector<Bits> sets;
vector<int> at[205];
Bits compatible[205];
int n,best;
int greedy(Bits r){int ans=0;while(count(r)){int j=-1,m=0;for(int i=0;i<(int)sets.size();i++){int t=count(inter(r,sets[i]));if(t>m)m=t,j=i;}r=minusB(r,sets[j]);ans++;}return ans;}
void dfs(Bits r,int used){int rem=count(r);if(!rem){best=min(best,used);return;}if(used+1>=best)return;
 int mx=0;for(int i=0;i<(int)sets.size();i++)mx=max(mx,count(inter(r,sets[i])));
 if(used+(rem+mx-1)/mx>=best)return;
 Bits pool=r;int pack=0;while(count(pool)){int v=-1,degree=n+1;for(int i=0;i<n;i++)if(pool.has(i)){int d=count(inter(pool,compatible[i]));if(d<degree)degree=d,v=i;}pack++;pool=minusB(pool,compatible[v]);}
 if(used+pack>=best)return;
 int v=-1,opts=1000000;for(int i=0;i<n;i++)if(r.has(i)){int k=at[i].size();if(k<opts)opts=k,v=i;}
 set<Bits> unique;vector<pair<int,Bits> > choices;
 for(int k=0;k<(int)at[v].size();k++){Bits s=inter(r,sets[at[v][k]]);if(unique.insert(s).second)choices.push_back(make_pair(-count(s),s));}
 sort(choices.begin(),choices.end());
 for(int k=0;k<(int)choices.size();k++)dfs(minusB(r,choices[k].second),used+1);
}
int main(){int tc;scanf("%d",&tc);while(tc--){double D;scanf("%d%lf",&n,&D);vector<Line> l(n);for(int i=0;i<n;i++){scanf("%lf%lf%lf",&l[i].a,&l[i].b,&l[i].c);double z=sqrt(l[i].a*l[i].a+l[i].b*l[i].b);l[i].a/=z;l[i].b/=z;l[i].c/=z;}
 set<Bits> uniq;for(int i=0;i<n;i++)for(int u=-1;u<=1;u+=2){double z=-l[i].c+u*D,x=l[i].a*z,y=l[i].b*z;Bits s;for(int k=0;k<n;k++)if(fabs(l[k].a*x+l[k].b*y+l[k].c)<=D+1e-8)s.add(k);uniq.insert(s);}
 for(int i=0;i<n;i++)for(int j=0;j<i;j++){double det=l[i].a*l[j].b-l[j].a*l[i].b;if(fabs(det)<1e-10)continue;for(int u=-1;u<=1;u+=2)for(int v=-1;v<=1;v+=2){double ci=l[i].c+u*D,cj=l[j].c+v*D;double x=(l[i].b*cj-l[j].b*ci)/det,y=(ci*l[j].a-cj*l[i].a)/det;Bits s;for(int k=0;k<n;k++)if(fabs(l[k].a*x+l[k].b*y+l[k].c)<=D+1e-8)s.add(k);uniq.insert(s);}}
 sets.assign(uniq.begin(),uniq.end());for(int i=0;i<n;i++){at[i].clear();compatible[i]=Bits();}
 for(int j=0;j<(int)sets.size();j++)for(int i=0;i<n;i++)if(sets[j].has(i)){at[i].push_back(j);for(int k=0;k<W;k++)compatible[i].x[k]|=sets[j].x[k];}
 Bits all;for(int i=0;i<n;i++)all.add(i);best=greedy(all);dfs(all,0);printf("%d\n",best);
 }return 0;}
