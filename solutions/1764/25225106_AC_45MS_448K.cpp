// POJ 1764 - Dice Contest
// Model: gpt-6.1-sol
// Close same-column paths under vertical moves and nested horizontal
// excursions, then exponentiate the one-column min-plus transition.
// The pictured net gives top/bottom 1/6, north/south 5/2, east/west 4/3.
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
typedef long long LL;
const int N=96, INF=100000000;
const LL BIG=1000000000000000LL;
int go[24][4], top[24], w[7];
struct Mat { LL a[N][N]; Mat(){for(int i=0;i<N;i++)for(int j=0;j<N;j++)a[i][j]=BIG;} };
Mat mul(const Mat &a,const Mat &b){Mat c;for(int i=0;i<N;i++)for(int k=0;k<N;k++)if(a.a[i][k]<BIG)for(int j=0;j<N;j++)if(a.a[i][k]+b.a[k][j]<c.a[i][j])c.a[i][j]=a.a[i][k]+b.a[k][j];return c;}
int main(){
 vector<vector<int> > o; map<vector<int>,int> ids;
 int init[]={1,6,5,2,4,3};o.push_back(vector<int>(init,init+6));ids[o[0]]=0;
 for(unsigned z=0;z<o.size();z++)for(int d=0;d<4;d++){
  vector<int> a=o[z],b=a;
  if(d==0){b[0]=a[5];b[1]=a[4];b[4]=a[0];b[5]=a[1];}
  if(d==1){b[0]=a[4];b[1]=a[5];b[4]=a[1];b[5]=a[0];}
  if(d==2){b[0]=a[3];b[1]=a[2];b[2]=a[0];b[3]=a[1];}
  if(d==3){b[0]=a[2];b[1]=a[3];b[2]=a[1];b[3]=a[0];}
  if(!ids.count(b)){int id=o.size();ids[b]=id;o.push_back(b);}go[z][d]=ids[b];
 }
 for(int i=0;i<24;i++)top[i]=o[i][0];
 for(int i=1;i<=6;i++)if(!(cin>>w[i]))return 0;
 LL x1,x2;int y1,y2;cin>>x1>>y1>>x2>>y2;--y1;--y2;
 int h[N][N];for(int i=0;i<N;i++)for(int j=0;j<N;j++)h[i][j]=(i==j?0:INF);
 for(int y=0;y<4;y++)for(int s=0;s<24;s++)for(int d=2;d<4;d++){
  int yy=y+(d==2?1:-1),t=go[s][d];if(yy>=0&&yy<4)h[y*24+s][yy*24+t]=w[top[t]];
 }
 // Every return to the same column is a concatenation of vertical moves
 // and a left/right step enclosing another same-column return.
 bool change=true;
 while(change){
  change=false;
  for(int k=0;k<N;k++)for(int i=0;i<N;i++)if(h[i][k]<INF)for(int j=0;j<N;j++)if(h[i][k]+h[k][j]<h[i][j]){h[i][j]=h[i][k]+h[k][j];change=true;}
  for(int i=0;i<N;i++)for(int j=0;j<N;j++)for(int d=0;d<2;d++){
   int si=go[i%24][d],sj=go[j%24][d];
   int v=w[top[si]]+h[i/24*24+si][j/24*24+sj]+w[top[j%24]];
   if(v<h[i][j]){h[i][j]=v;change=true;}
  }
 }
 int dir=(x2>=x1?0:1);LL dist=x2-x1;if(dist<0)dist=-dist;
 Mat a;for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(h[i][j]<INF){int t=go[j%24][dir];a.a[i][j/24*24+t]=h[i][j]+w[top[t]];}
 LL v[N],u[N];for(int i=0;i<N;i++)v[i]=BIG;v[y1*24]=0;
 while(dist){if(dist&1){for(int j=0;j<N;j++){u[j]=BIG;for(int i=0;i<N;i++)u[j]=min(u[j],v[i]+a.a[i][j]);}for(int j=0;j<N;j++)v[j]=u[j];}dist/=2;if(dist)a=mul(a,a);}
 LL ans=BIG;for(int i=0;i<N;i++)for(int s=0;s<24;s++)ans=min(ans,v[i]+h[i][y2*24+s]);cout<<ans<<endl;
}
