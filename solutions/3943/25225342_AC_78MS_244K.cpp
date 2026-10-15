// POJ 3943 - Digits on the Floor
// Model: gpt-6.1-sol
// Find connected components of touching bars, then match the ten templates.
// Relations record endpoint/interior contacts and signed right angles; this
// preserves rotations and length changes but distinguishes mirrored 2 and 5.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct P { int x,y; P(int X=0,int Y=0):x(X),y(Y){} };
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
int cross(P a,P b){return a.x*b.y-a.y*b.x;}
int dot(P a,P b){return a.x*b.x+a.y*b.y;}
struct B {P p[2]; B(int a=0,int b=0,int c=0,int d=0){p[0]=P(a,b);p[1]=P(c,d);} };
int pos(P p,B b){if(cross(p-b.p[0],b.p[1]-b.p[0]) || dot(p-b.p[0],p-b.p[1])>0)return 0; if(p.x==b.p[0].x&&p.y==b.p[0].y)return 1; if(p.x==b.p[1].x&&p.y==b.p[1].y)return 3; return 2;}
int rel(B a,B b){int x=0,y=0; for(int k=0;k<2;k++){int t=pos(a.p[k],b);if(t){x=k?3:1;y=t;}} for(int k=0;k<2;k++){int t=pos(b.p[k],a);if(t){x=t;y=k?3:1;}} if(!x)return 0; return x*10+y+(cross(a.p[1]-a.p[0],b.p[1]-b.p[0])>0?100:200);}
int ar[5][5][2][2],tr[5][5],m,mp[5],fl[5];bool used[5];
bool dfs(int k){if(k==m)return true;for(int i=0;i<m;i++)if(!used[i])for(int f=0;f<2;f++){bool ok=true;for(int j=0;j<k;j++)if(ar[i][mp[j]][f][fl[j]]!=tr[k][j]){ok=false;break;}if(ok){used[i]=true;mp[k]=i;fl[k]=f;if(dfs(k+1))return true;used[i]=false;}}return false;}
vector<B> ts[10];
void add(int d,int a,int b,int c,int e){ts[d].push_back(B(a,b,c,e));}
void init(){
add(0,0,0,0,2);add(0,0,2,1,2);add(0,1,2,1,0);add(0,1,0,0,0);
add(1,0,0,0,2);
add(2,0,2,1,2);add(2,1,2,1,1);add(2,1,1,0,1);add(2,0,1,0,0);add(2,0,0,1,0);
add(3,0,2,1,2);add(3,0,1,1,1);add(3,0,0,1,0);add(3,1,0,1,2);
add(4,0,2,0,1);add(4,0,1,1,1);add(4,1,0,1,2);
add(5,1,2,0,2);add(5,0,2,0,1);add(5,0,1,1,1);add(5,1,1,1,0);add(5,1,0,0,0);
add(6,0,2,1,2);add(6,0,2,0,0);add(6,0,1,1,1);add(6,1,1,1,0);add(6,1,0,0,0);
add(7,0,1,0,2);add(7,0,2,1,2);add(7,1,2,1,0);
add(8,0,0,0,2);add(8,1,0,1,2);add(8,0,0,1,0);add(8,0,1,1,1);add(8,0,2,1,2);
add(9,0,2,1,2);add(9,0,2,0,1);add(9,0,1,1,1);add(9,1,0,1,2);
}
int main(){init();int n;while(cin>>n&&n){vector<B>b(n);for(int i=0;i<n;i++)cin>>b[i].p[0].x>>b[i].p[0].y>>b[i].p[1].x>>b[i].p[1].y;vector<vector<int> >g(n);for(int i=0;i<n;i++)for(int j=0;j<i;j++)if(rel(b[i],b[j])){g[i].push_back(j);g[j].push_back(i);}vector<int>vis(n,0);int ans[10]={0};for(int s=0;s<n;s++)if(!vis[s]){vector<int>v(1,s);vis[s]=1;for(unsigned k=0;k<v.size();k++)for(unsigned j=0;j<g[v[k]].size();j++){int z=g[v[k]][j];if(!vis[z]){vis[z]=1;v.push_back(z);}}m=v.size();if(m>5)continue;for(int i=0;i<m;i++)for(int j=0;j<m;j++)for(int f=0;f<2;f++)for(int h=0;h<2;h++){B a=b[v[i]],c=b[v[j]];if(f)swap(a.p[0],a.p[1]);if(h)swap(c.p[0],c.p[1]);ar[i][j][f][h]=rel(a,c);}for(int d=0;d<10;d++)if((int)ts[d].size()==m){for(int i=0;i<m;i++){used[i]=false;for(int j=0;j<m;j++)tr[i][j]=rel(ts[d][i],ts[d][j]);}if(dfs(0)){ans[d]++;break;}}}for(int d=0;d<10;d++)cout<<(d?" ":"")<<ans[d];cout<<"\n";}}
