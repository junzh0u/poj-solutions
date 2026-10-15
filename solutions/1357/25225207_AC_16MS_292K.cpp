// POJ 1357 - Afshung Pizza Delivery
// Model: gpt-6.1-sol
// Compress straight streets, counting only '-' and '|' as travel time.
// Dijkstra states retain the arrival side; at a light, wait for that
// side's phase in the counterclockwise cycle of existing approaches.
// Dead-end streets still participate in the signal cycle.
// S, D, turns and intersections have zero length; incompatible street
// characters block a trace, so an unmarked crossing is not connected.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
using namespace std;
struct Node { int r,c,t; char ch; int to[4],len[4],phase[4],degree; };
int dr[4]={1,0,-1,0}, dc[4]={0,1,0,-1};
int main(){
 int cases; cin>>cases;
 while(cases--){
  int n,m;cin>>n>>m;string line;getline(cin,line);
  vector<string> a(n);vector<Node> v;int ids[100][100];int s=-1,dest=-1;
  for(int r=0;r<n;++r){getline(cin,a[r]);if(!a[r].empty()&&a[r][a[r].size()-1]=='\r')a[r].erase(a[r].size()-1);a[r].resize(m,' ');
   for(int c=0;c<m;++c){ids[r][c]=-1;char ch=a[r][c];if(ch=='S'||ch=='D'||ch=='+'||(ch>='1'&&ch<='9')){Node x;x.r=r;x.c=c;x.ch=ch;x.t=(ch>='1'&&ch<='9')?ch-'0':0;x.degree=0;for(int k=0;k<4;++k){x.to[k]=-1;x.len[k]=0;x.phase[k]=-1;}ids[r][c]=v.size();if(ch=='S')s=v.size();if(ch=='D')dest=v.size();v.push_back(x);}}
  }
  for(int i=0;i<(int)v.size();++i)for(int k=0;k<4;++k){
   int r=v[i].r+dr[k],c=v[i].c+dc[k],len=0;
   while(r>=0&&r<n&&c>=0&&c<m){char ch=a[r][c];if(ids[r][c]>=0){v[i].to[k]=ids[r][c];v[i].len[k]=len;break;}if(ch!=((k%2==0)?'|':'-'))break;++len;r+=dr[k];c+=dc[k];}
  }
  for(int i=0;i<(int)v.size();++i){bool exists[4];for(int k=0;k<4;++k){int r=v[i].r+dr[k],c=v[i].c+dc[k];exists[k]=r>=0&&r<n&&c>=0&&c<m&&a[r][c]==((k%2==0)?'|':'-');}int start=exists[0]?0:2;for(int j=0;j<4;++j){int k=(start+j)%4;if(exists[k])v[i].phase[k]=v[i].degree++;}}
  const int INF=1000000000;vector<int> dist(v.size()*4,INF);typedef pair<int,int> P;priority_queue<P,vector<P>,greater<P> > q;
  for(int k=0;k<4;++k){dist[s*4+k]=0;q.push(P(0,s*4+k));}
  int ans=-1;
  while(!q.empty()){int tm=q.top().first,z=q.top().second;q.pop();if(tm!=dist[z])continue;int i=z/4,side=z%4;if(i==dest){ans=tm;break;}
   int leave=tm;if(v[i].t){int cycle=v[i].degree*v[i].t;int target=v[i].phase[side]*v[i].t;int rem=tm%cycle;if(rem<target)leave+=target-rem;else if(rem>=target+v[i].t)leave+=cycle-rem+target;}
   for(int k=0;k<4;++k)if(v[i].to[k]>=0){int j=v[i].to[k],nz=j*4+(k+2)%4,nt=leave+v[i].len[k];if(nt<dist[nz]){dist[nz]=nt;q.push(P(nt,nz));}}
  }
  if(ans<0)cout<<"impossible\n";else cout<<ans<<"\n";
 }
}
