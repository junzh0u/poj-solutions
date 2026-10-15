// POJ 3679 - The Delivery of Control
// Model: gpt-6.1-sol
// Enumerate every simple lattice polygon of perimeter at most T, once up
// to translation, starting east from its leftmost topmost vertex.
// Signed vertical edges integrate row prefix sums over exactly its interior;
// in particular an enclosed negative block cannot be omitted or made a hole.
#include <cstdio>
#include <algorithm>
#include <cstdlib>
using namespace std;
int n,m,t,px[22],py[22],vis[43][23];
long long pref[21][21],ans=0;
int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};
void evaluate(int len,int lo,int hi,int h){
    int er[22],ex[22],sg[22],cnt=0;
    for(int i=0;i<len;++i){
        int j=(i+1)%len;
        if(py[i]!=py[j]){
            er[cnt]=min(py[i],py[j]); ex[cnt]=px[i];
            sg[cnt++]=py[j]>py[i]?1:-1;
        }
    }
    for(int y=0;y+h<=n;++y)for(int x=-lo;x+hi<=m;++x){
        long long v=0;
        for(int k=0;k<cnt;++k)v+=sg[k]*pref[y+er[k]][x+ex[k]];
        if(v>ans)ans=v;
    }
}
void dfs(int x,int y,int len,int lo,int hi,int h){
    if(len-1+abs(x)+y>t)return;
    for(int d=0;d<4;++d){
        int a=x+dx[d],b=y+dy[d];
        if(a==0&&b==0){if(len>=4)evaluate(len,lo,hi,h);continue;}
        if(len==t||b<0||(b==0&&a<0)||b>n||abs(a)>m||vis[a+21][b])continue;
        int l=min(lo,a),r=max(hi,a),hh=max(h,b);
        if(r-l>m||hh>n)continue;
        vis[a+21][b]=1;px[len]=a;py[len]=b;
        dfs(a,b,len+1,l,r,hh);
        vis[a+21][b]=0;
    }
}
int main(){
    if(scanf("%d%d%d",&n,&m,&t)!=3)return 0;
    for(int i=0;i<n;++i)for(int j=1;j<=m;++j){int a;scanf("%d",&a);pref[i][j]=pref[i][j-1]+a;}
    vis[21][0]=vis[22][0]=1;px[1]=1;
    if(t>=4)dfs(1,0,2,0,1,0);
    printf("%lld\n",ans);return 0;
}
