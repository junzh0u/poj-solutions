// POJ 2929 - The Traveling Salesman
// Model: gpt-6.1-sol
// Grow the visited northwest rectangle by complete rows or columns.
// Track its endpoint corner; canonicalize coincident corners on thin grids.
// Prefix sums check bridges and elevator costs. Counts use arbitrary precision.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
const int K=1002, INF=1000000000;
int a[K][K], hc[K][K],vc[K][K],hb[K][K],vb[K][K];
struct Big { vector<int> d; Big(int x=0){if(x)d.push_back(x);} void add(const Big& b){int carry=0;size_t n=max(d.size(),b.d.size());d.resize(n,0);for(size_t i=0;i<n;i++){int v=d[i]+(i<b.d.size()?b.d[i]:0)+carry;d[i]=v%1000000000;carry=v/1000000000;}if(carry)d.push_back(carry);} void print()const{if(d.empty()){printf("0");return;}printf("%d",d.back());for(int i=(int)d.size()-2;i>=0;i--)printf("%09d",d[i]);}};
struct State {int cost;Big count;State():cost(INF){} void relax(int c,const Big& b){if(c<cost){cost=c;count=b;}else if(c==cost)count.add(b);}};
State dp[2][K][3];
int corner(int x,int y,int w,int h){if(x==w-1&&y==0)return 0;if(x==0&&y==h-1)return 2;return 1;}
int main(){int m,n;if(scanf("%d%d",&m,&n)!=2)return 0;char token[100];int y=1,x=1,lastx=0,lasty=0;while(scanf("%99s",token)==1){if(token[0]=='x'){hb[lasty][lastx+1]=1;continue;}if(token[0]=='y'){vb[lasty+1][lastx]=1;continue;}if(y>m)break;a[y][x]=atoi(token);lastx=x;lasty=y;if(++x>n){x=1;y++;}}
for(y=1;y<=m;y++)for(x=1;x<=n;x++){hc[y][x]=hc[y][x-1]+(x>1?abs(a[y][x]-a[y][x-1]):0);hb[y][x]+=hb[y][x-1];vc[y][x]=vc[y-1][x]+(y>1?abs(a[y][x]-a[y-1][x]):0);vb[y][x]+=vb[y-1][x];}
dp[1][1][0].cost=a[1][1];dp[1][1][0].count=Big(1);State ans;
for(int h=1;h<=m;h++){int cur=h%2,next=1-cur;for(int w=1;w<=n;w++)for(int s=0;s<3;s++){State &v=dp[cur][w][s];if(v.cost==INF)continue;int xx=s==2?0:w-1, yy=s==0?0:h-1;if(corner(xx,yy,w,h)!=s)continue;if(h==m&&w==n){ans.relax(v.cost+a[yy+1][xx+1],v.count);continue;}
if(w<n&&xx==w-1){int row=yy+1;if(hb[row][w+1]-hb[row][w]==0&&vb[h][w+1]==0){int ny=h-1-yy;int ns=corner(w,ny,w+1,h);int c=abs(a[row][w]-a[row][w+1])+vc[h][w+1];dp[cur][w+1][ns].relax(v.cost+c,v.count);}}
if(h<m&&yy==h-1){int col=xx+1;if(vb[h+1][col]-vb[h][col]==0&&hb[h+1][w]==0){int nx=w-1-xx;int ns=corner(nx,h,w,h+1);int c=abs(a[h][col]-a[h+1][col])+hc[h+1][w];dp[next][w][ns].relax(v.cost+c,v.count);}}
}for(int w=1;w<=n;w++)for(int s=0;s<3;s++)dp[cur][w][s]=State();}
if(ans.cost==INF)puts("No solution");else{ans.count.print();printf(" tours, traveling a minimum of %d total floors\n",ans.cost);}return 0;}
