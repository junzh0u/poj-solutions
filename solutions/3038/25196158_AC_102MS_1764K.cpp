// POJ 3038 - Airline
// Model: gpt-5.6-terra
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;struct G{int s,e,m;bool operator<(const G&o)const{return e<o.e;}};int mn[80005],lz[80005];void add(int p,int l,int r,int a,int b,int v){if(a<=l&&r<=b){mn[p]+=v;lz[p]+=v;return;}int m=(l+r)/2;if(a<=m)add(p*2,l,m,a,b,v);if(b>m)add(p*2+1,m+1,r,a,b,v);mn[p]=lz[p]+min(mn[p*2],mn[p*2+1]);}int get(int p,int l,int r,int a,int b){if(a<=l&&r<=b)return mn[p];int m=(l+r)/2,x=1<<30;if(a<=m)x=min(x,lz[p]+get(p*2,l,m,a,b));if(b>m)x=min(x,lz[p]+get(p*2+1,m+1,r,a,b));return x;}int main(){int k,n,c;cin>>k>>n>>c;vector<G>a,b;for(int i=0;i<k;i++){G g;cin>>g.s>>g.e>>g.m;if(g.s<g.e)a.push_back(g);else{swap(g.s,g.e);b.push_back(g);}}long long ans=0;for(int z=0;z<2;z++){vector<G>&v=z?b:a;sort(v.begin(),v.end());fill(mn,mn+80005,c);fill(lz,lz+80005,0);for(int i=0;i<(int)v.size();i++){int x=min(v[i].m,get(1,1,n-1,v[i].s,v[i].e-1));ans+=x;add(1,1,n-1,v[i].s,v[i].e-1,-x);}}cout<<ans<<'\n';}
