// POJ 3242 - 3-dimensional Musical Water-fence
// Model: gpt-6.1-sol
// Priority flood finds spill levels; connected interiors remain separate pools.
// Process spill blocks downward, sharing incoming and overflowing water among
// lower exits and unfilled pools in proportion to their neighboring faces.
// Remove each full pool from the split and route the remainder downstream.
// Aggregate pours because the capped proportional allocation is order invariant.
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Item { int h,v; Item(int a,int b):h(a),v(b){} bool operator<(const Item& o)const{return h>o.h;} };
int main(){
 int n,m; if(scanf("%d%d",&n,&m)!=2)return 0; int N=n*m;
 vector<int> h(N),lev(N,-1),pos(N+1,-1);
 for(int i=0;i<N;++i){scanf("%d",&h[i]);pos[h[i]]=i;}
 priority_queue<Item> pq;
 for(int i=0;i<N;++i)if(i<m||i>=N-m||i%m==0||i%m==m-1){lev[i]=h[i];pq.push(Item(h[i],i));}
 int off[4]={-m,m,-1,1};
 while(!pq.empty()) {Item a=pq.top();pq.pop();for(int d=0;d<4;++d){int v=a.v+off[d];if(v<0||v>=N||(d==2&&a.v%m==0)||(d==3&&a.v%m==m-1))continue;if(lev[v]<0){lev[v]=max(h[v],a.h);pq.push(Item(lev[v],v));}}}
 vector<int> basin(N,-1),q;
 vector<double> cap;
 vector<long double> inside;
 for(int i=0;i<N;++i)if(h[i]<lev[i]&&basin[i]<0){
  int b=cap.size();cap.push_back(0);inside.push_back(0);q.clear();q.push_back(i);basin[i]=b;
  for(unsigned z=0;z<q.size();++z){int v=q[z];cap[b]+=lev[v]-h[v];for(int d=0;d<4;++d){int u=v+off[d];if(u<0||u>=N||(d==2&&v%m==0)||(d==3&&v%m==m-1))continue;if(basin[u]<0&&lev[u]==lev[v]&&h[u]<lev[u]){basin[u]=b;q.push_back(u);}}}
 }
 vector<int>().swap(q);
 vector<long double> top(N+1,0);
 long double total=0,retained=0;
 int t;scanf("%d",&t);for(int z=0;z<t;++z){int i,j,k;scanf("%d%d%d",&i,&j,&k);int v=(i-1)*m+j-1;total+=k;if(basin[v]<0)top[h[v]]+=k;else inside[basin[v]]+=k;}
 for(int l=N;l>=1;--l){int v=pos[l];if(v<0||lev[v]!=l)continue;
  int dest[4],dc=0,bs[4],weight[4],bc=0;
  for(int d=0;d<4;++d){int u=v+off[d];if(u<0||u>=N||(d==2&&v%m==0)||(d==3&&v%m==m-1)){dest[dc++]=-1;}else if(h[u]<l){if(lev[u]<l)dest[dc++]=u;else{int b=basin[u],j=0;while(j<bc&&bs[j]!=b)++j;if(j==bc){bs[bc]=b;weight[bc++]=0;}++weight[j];}}}
  long double flow=top[l];
  for(int j=0;j<bc;++j){int b=bs[j];if(inside[b]>=cap[b]){flow+=inside[b]-cap[b];inside[b]=cap[b];weight[j]=0;}}
  long double out=0;
  while(flow>0){int count=dc;for(int j=0;j<bc;++j)count+=weight[j];if(!count)break;
   long double per=flow/count;
   for(int j=0;j<bc;++j)if(weight[j])per=min(per,(cap[bs[j]]-inside[bs[j]])/weight[j]);
   out+=per*dc;flow-=per*count;
   bool filled=false;
   for(int j=0;j<bc;++j)if(weight[j]){inside[bs[j]]+=per*weight[j];if(cap[bs[j]]-inside[bs[j]]<1e-10L){inside[bs[j]]=cap[bs[j]];weight[j]=0;filled=true;}}
   if(!filled)break;
  }
  for(int j=0;j<bc;++j)retained+=inside[bs[j]];
  for(int j=0;j<dc;++j){int u=dest[j];if(u<0)continue;long double w=out/dc;if(basin[u]<0)top[h[u]]+=w;else inside[basin[u]]+=w;}
 }
 printf("%.2Lf\n",total-retained);return 0;
}
