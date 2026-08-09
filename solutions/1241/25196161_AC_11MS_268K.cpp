// POJ 1241 - Tournament
// Model: gpt-5.6-terra
#include <iostream>
#include <vector>
using namespace std;int dfs(int x,vector<vector<int> >&ch,vector<int>&sz){sz[x]=1;for(int i=0;i<(int)ch[x].size();i++)sz[x]+=dfs(ch[x][i],ch,sz);return sz[x];}int main(){int n,tc=0;while(cin>>n&&n){int N=1<<n;vector<int>p(N+1),a(N),sz(N+1);vector<vector<int> >ch(N+1);for(int i=0;i<N;i++)a[i]=i+1;for(int r=0,c=N/2;r<n;r++,c/=2){vector<int>b(c);for(int i=0;i<c;i++){cin>>b[i];int x=a[2*i],y=a[2*i+1],z=b[i],lose=(z==x?y:x);p[lose]=z;ch[z].push_back(lose);}a=b;}dfs(a[0],ch,sz);int m,k;if(tc++)cout<<'\n';cin>>m;while(m--){cin>>k;int d=0;for(int x=k;p[x];x=p[x])d++;cout<<"Player "<<k<<" can be ranked as high as "<<d+1<<" or as low as "<<N-sz[k]+1<<".\n";}}}
