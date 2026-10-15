// POJ 3248 - Soldier
// Model: gpt-6.1-sol
// Split the rotated integer lattice by parity, then sweep rectangle events.
// A segment tree stores the count and coordinate sum of covered ordinates.
// Board clipping gives a piecewise linear weight for each sweep slab.
// Coordinates are one-based; coincident soldiers contribute only their union.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long LL;
LL fd(LL x,LL y){LL q=x/y; if(x%y<0)--q;return q;}
LL cd(LL x,LL y){return -fd(-x,y);}
struct Soldier{LL x,y,r;};
struct Event{LL a,l,r;int d;bool operator<(const Event&o)const{return a<o.a;}};
vector<LL> z,ct,sm;vector<int> cover;
LL series(LL l,LL r){return (l+r)*(r-l+1)/2;}
void pull(int t,int l,int r){if(cover[t]){ct[t]=z[r+1]-z[l];sm[t]=series(z[l],z[r+1]-1);}else if(l==r)ct[t]=sm[t]=0;else{ct[t]=ct[t*2]+ct[t*2+1];sm[t]=sm[t*2]+sm[t*2+1];}}
void upd(int t,int l,int r,int a,int b,int d){if(a<=l&&r<=b)cover[t]+=d;else{int m=(l+r)/2;if(a<=m)upd(t*2,l,m,a,b,d);if(b>m)upd(t*2+1,m+1,r,a,b,d);}pull(t,l,r);}
LL query(int t,int l,int r,LL a,LL b,LL slope,LL intercept){if(!ct[t]||b<z[l]||a>=z[r+1])return 0;if(a<=z[l]&&z[r+1]-1<=b)return slope*sm[t]+intercept*ct[t];if(cover[t]){LL lo=max(a,z[l]),hi=min(b,z[r+1]-1);return slope*series(lo,hi)+intercept*(hi-lo+1);}int m=(l+r)/2;return query(t*2,l,m,a,b,slope,intercept)+query(t*2+1,m+1,r,a,b,slope,intercept);}
LL n,m;
LL weight(LL b,LL L,LL R,int p){return min(R,min(n-p-b,m+b))-max(L,max(1-p-b,1+b))+1;}
LL slab(LL L,LL R,int p){
 LL s[6]={0,-1,1,0,-1,1},c[6]={R,n-p,m,L,1-p,1};
 vector<LL> cuts;cuts.push_back(z.front());cuts.push_back(z.back());
 for(int base=0;base<6;base+=3)for(int i=base;i<base+3;++i)for(int j=i+1;j<base+3;++j){LL d=s[i]-s[j],v=c[j]-c[i];if(d<0){d=-d;v=-v;}LL q=fd(v,d);if(q>=z.front()&&q<z.back())cuts.push_back(q+1);}
 sort(cuts.begin(),cuts.end());cuts.erase(unique(cuts.begin(),cuts.end()),cuts.end());LL ans=0;
 for(size_t i=1;i<cuts.size();++i){LL l=cuts[i-1],r=cuts[i]-1,w=weight(l,L,R,p),a=(l<r?weight(l+1,L,R,p)-w:0),b=w-a*l;
 if(a>0)l=max(l,fd(-b,a)+1);else if(a<0)r=min(r,cd(b,-a)-1);else if(b<=0)continue;
 if(l<=r)ans+=query(1,0,(int)z.size()-2,l,r,a,b);
 }return ans;
}
int main(){ios::sync_with_stdio(false);int k;if(!(cin>>n>>m>>k))return 0;vector<Soldier> sol(k);for(int i=0;i<k;++i)cin>>sol[i].x>>sol[i].y>>sol[i].r;LL answer=0;
 for(int p=0;p<2;++p){vector<Event> e;z.clear();LL amin=cd(2-p,2),amax=fd(n+m-p,2),bmin=cd(1-m-p,2),bmax=fd(n-1-p,2);
 for(int i=0;i<k;++i){LL u=sol[i].x+sol[i].y,v=sol[i].x-sol[i].y,r=sol[i].r;LL a=max(amin,cd(u-r-p,2)),b=min(amax,fd(u+r-p,2)),lo=max(bmin,cd(v-r-p,2)),hi=min(bmax,fd(v+r-p,2));if(a>b||lo>hi)continue;Event t;t.a=a;t.l=lo;t.r=hi+1;t.d=1;e.push_back(t);t.a=b+1;t.d=-1;e.push_back(t);z.push_back(lo);z.push_back(hi+1);}
 if(e.empty())continue;sort(e.begin(),e.end());sort(z.begin(),z.end());z.erase(unique(z.begin(),z.end()),z.end());ct.assign(z.size()*4,0);sm.assign(z.size()*4,0);cover.assign(z.size()*4,0);
 size_t i=0;while(i<e.size()){LL a=e[i].a;while(i<e.size()&&e[i].a==a){int l=lower_bound(z.begin(),z.end(),e[i].l)-z.begin(),r=lower_bound(z.begin(),z.end(),e[i].r)-z.begin()-1;upd(1,0,(int)z.size()-2,l,r,e[i].d);++i;}if(i<e.size()&&ct[1])answer+=slab(a,e[i].a-1,p);}
 }cout<<answer<<'\n';}
