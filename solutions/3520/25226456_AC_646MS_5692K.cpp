// POJ 3520 - Lowest Pyramid
// Model: gpt-6-astra
// Enumerate lattice vertices outside each base edge and join them by the
// squared lengths of the three edges to the folded apex. Trilateration
// gives the height; exact integer separating axes exclude overlapping nets.
// Boundary contact is allowed, but faces and the pyramid must be nondegenerate.
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;
struct P { int x,y; P(int a=0,int b=0):x(a),y(b){} };
int cross(P a,P b,P c){return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);}
int dist(P a,P b){int x=a.x-b.x,y=a.y-b.y;return x*x+y*y;}
bool separate(P *a,P *b){
    for(int i=0;i<3;++i){
        int lo=1000000000,hi=-lo;
        for(int j=0;j<3;++j){int v=cross(a[i],a[(i+1)%3],b[j]);lo=min(lo,v);hi=max(hi,v);}
        int side=cross(a[i],a[(i+1)%3],a[(i+2)%3]);
        if((side>0 && hi<=0)||(side<0 && lo>=0))return true;
    }return false;
}
bool overlap(P *a,P *b){return !separate(a,b)&&!separate(b,a);}
vector<P> bydist[80001];
int main(){
 P v[3];
 while(scanf("%d%d%d%d%d%d",&v[0].x,&v[0].y,&v[1].x,&v[1].y,&v[2].x,&v[2].y)==6){
  if(v[0].x==0&&v[0].y==0&&v[1].x==0&&v[1].y==0&&v[2].x==0&&v[2].y==0)break;
  vector<P> first; map<pair<int,int>,P> third;
  for(int i=0;i<=80000;++i)bydist[i].clear();
  for(int x=-100;x<=100;++x)for(int y=-100;y<=100;++y){P p(x,y);
   if(cross(v[0],v[1],p)<0)first.push_back(p);
   if(cross(v[1],v[2],p)<0)bydist[dist(v[1],p)].push_back(p);
   if(cross(v[2],v[0],p)<0)third[make_pair(dist(v[0],p),dist(v[2],p))]=p;
  }
  long double den=2*cross(v[0],v[1],v[2]), best=1e100L;
  for(size_t i=0;i<first.size();++i){
   P a=first[i]; int r0=dist(a,v[0]),r1=dist(a,v[1]);
   const vector<P>& bs=bydist[r1];
   for(size_t j=0;j<bs.size();++j){P b=bs[j];int r2=dist(b,v[2]);
    long double u=r0-r1+dist(v[0],v[1]),w=r0-r2+dist(v[0],v[2]);
    long double nx=u*(v[2].y-v[0].y)-w*(v[1].y-v[0].y);
    long double ny=w*(v[1].x-v[0].x)-u*(v[2].x-v[0].x);
    long double hnum=r0*den*den-nx*nx-ny*ny;
    if(hnum<=0 || hnum>=best)continue;
    map<pair<int,int>,P>::iterator it=third.find(make_pair(r0,r2));
    if(it==third.end())continue;
    P c=it->second,t0[3]={v[0],v[1],a},t1[3]={v[1],v[2],b},t2[3]={v[2],v[0],c};
    if(overlap(t0,t1)||overlap(t1,t2)||overlap(t2,t0))continue;
    best=hnum;
   }
  }
  if(best==1e100L)puts("-1");else printf("%.10f\n",sqrt((double)(best/(den*den))));
 }
 return 0;
}
