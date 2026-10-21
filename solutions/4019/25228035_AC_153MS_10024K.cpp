// POJ 4019 - Immediate Delivery
// Model: gpt-6-astra
// Floyd shortest paths and subset endpoint DP, then minimize the maximum
// over complementary delivery sets. Expand metric edges into real roads.
// Both routes start at 1 and may finish anywhere; intermediate visits are legal.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=1000000000;
int n,k,d[18][18],nx[18][18];
vector<int> dp,best,last;
vector<int> route(int mask){
    vector<int> rev,ans(1,0);
    int v=last[mask];
    while(mask){
        rev.push_back(v+1);
        int prev=mask^(1<<v),u;
        if(!prev) break;
        for(u=0;u<k;++u) if((prev&(1<<u)) && dp[prev*k+u]+d[u+1][v+1]==dp[mask*k+v]) break;
        mask=prev; v=u;
    }
    reverse(rev.begin(),rev.end());
    for(size_t i=0;i<rev.size();++i){
        int u=ans.back(),vtx=rev[i];
        while(u!=vtx){u=nx[u][vtx];ans.push_back(u);}
    }
    return ans;
}
void print(const vector<int>& a){
    printf("%d",int(a.size())-1);
    for(size_t i=0;i<a.size();++i) printf(" %d",a[i]+1);
    puts("");
}
int main(){
    int m;
    if(scanf("%d%d",&n,&m)!=2)return 0;
    for(int i=0;i<n;++i)for(int j=0;j<n;++j){d[i][j]=(i==j?0:INF);nx[i][j]=j;}
    for(int i=0;i<m;++i){int u,v,w;scanf("%d%d%d",&u,&v,&w);--u;--v;d[u][v]=d[v][u]=w;}
    for(int h=0;h<n;++h)for(int i=0;i<n;++i)for(int j=0;j<n;++j)
        if(d[i][h]+d[h][j]<d[i][j]){d[i][j]=d[i][h]+d[h][j];nx[i][j]=nx[i][h];}
    k=n-1;int count=1<<k;
    dp.assign(count*k,INF);best.assign(count,INF);last.assign(count,-1);best[0]=0;
    for(int mask=1;mask<count;++mask){
        for(int v=0;v<k;++v)if(mask&(1<<v)){
            int prev=mask^(1<<v),val=INF;
            if(!prev)val=d[0][v+1];
            else for(int u=0;u<k;++u)if(prev&(1<<u))val=min(val,dp[prev*k+u]+d[u+1][v+1]);
            dp[mask*k+v]=val;
            if(val<best[mask]){best[mask]=val;last[mask]=v;}
        }
    }
    int answer=INF,split=0;
    for(int mask=0;mask<count;++mask){int val=max(best[mask],best[(count-1)^mask]);if(val<answer){answer=val;split=mask;}}
    printf("%d\n",answer);print(route(split));print(route((count-1)^split));
}
