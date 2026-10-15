// POJ 3655 - Guard
// Model: gpt-6.1-sol
// Compute the optimal one-guard risk for every subset, then partition subsets.
// A guard is either at a listed junction, seeing all incident corridors, or
// inside a single corridor. On a line, weighted distance intervals intersect
// exactly when every pair intersects; the largest pair bound is the optimum.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;
const double INF=1e30;
int main(){
 int p,c,g;
 while(cin>>p && p){
  cin>>c>>g;
  double x[12],y[12],v[12],d[12][12]; int item[12],n=0,cor[12];
  for(int i=0;i<p;++i){ char label; double a,b,w; cin>>label>>a>>b>>w;
   int k=label-'A'; x[k]=a;y[k]=b;v[k]=w;
  }
  for(int i=0;i<p;++i) if(v[i]>0) item[n++]=i;
  for(int i=0;i<c;++i){ string s;cin>>s;cor[i]=0;
   for(unsigned j=0;j<s.size();++j)cor[i]|=1<<(s[j]-'A');
  }
  for(int i=0;i<p;++i)for(int j=0;j<p;++j){double a=x[i]-x[j],b=y[i]-y[j];d[i][j]=sqrt(a*a+b*b);}
  int N=1<<n; double cost[2048],prev[2048],next[2048];
  cost[0]=0;
  for(int mask=1;mask<N;++mask){
   cost[mask]=INF;
   for(int pos=0;pos<p;++pos){
    int visible=0;for(int k=0;k<c;++k)if(cor[k]&(1<<pos))visible|=cor[k];
    double r=0;bool ok=true;
    for(int i=0;i<n;++i)if(mask&(1<<i)){
     if(!(visible&(1<<item[i]))) {ok=false;break;}
     r=max(r,v[item[i]]*d[pos][item[i]]);
    }
    if(ok)cost[mask]=min(cost[mask],r);
   }
   for(int k=0;k<c;++k){
    bool ok=true;for(int i=0;i<n;++i)if((mask&(1<<i)) && !(cor[k]&(1<<item[i])))ok=false;
    if(!ok)continue;
    double r=0;
    for(int i=0;i<n;++i)if(mask&(1<<i))for(int j=0;j<i;++j)if(mask&(1<<j)){
     int a=item[i],b=item[j];r=max(r,d[a][b]*v[a]*v[b]/(v[a]+v[b]));
    }
    cost[mask]=min(cost[mask],r);
   }
  }
  for(int m=0;m<N;++m)prev[m]=cost[m];
  for(int guards=2;guards<=g;++guards){
   next[0]=0;
   for(int m=1;m<N;++m){
    next[m]=prev[m];int bit=m&-m;
    for(int s=m;s;s=(s-1)&m)if(s&bit)next[m]=min(next[m],max(cost[s],prev[m^s]));
   }
   for(int m=0;m<N;++m)prev[m]=next[m];
  }
  if(prev[N-1]>INF/2)puts("too few guards");else printf("%.2f\n",prev[N-1]);
 }
 return 0;
}
