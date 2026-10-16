// POJ 2999 - Matrix
// Model: gpt-6.1-sol
// Retrograde analysis of (human, agent1, agent2, turn) states.
// Staying still is legal. Capture takes precedence over an occupied phone.
// Unresolved states are draws: neither side can force its terminal goal.
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;
int n;
int encode(int m,int a,int b,int t){return (((m*n+a)*n+b)*2+t);}
int main(){
 int cases; cin>>cases;
 while(cases--){
  string board[8]; int id[8][8],m=0,a=0,b=0,ac=0; n=0;
  for(int i=0;i<8;++i) cin>>board[i];
  vector<int> phone;
  for(int i=0;i<8;++i)for(int j=0;j<8;++j){
   id[i][j]=-1;
   if(board[i][j]!='#'){
    id[i][j]=n++; phone.push_back(board[i][j]=='P');
    if(board[i][j]=='M')m=n-1;
    if(board[i][j]=='A'){if(ac++==0)a=n-1;else b=n-1;}
   }
  }
  vector< vector<int> > adj(n);
  int dx[5]={0,1,-1,0,0},dy[5]={0,0,0,1,-1};
  for(int i=0;i<8;++i)for(int j=0;j<8;++j)if(id[i][j]>=0)
   for(int d=0;d<5;++d){int x=i+dx[d],y=j+dy[d];
    if(x>=0&&x<8&&y>=0&&y<8&&id[x][y]>=0)adj[id[i][j]].push_back(id[x][y]);
   }
  int count=n*n*n*2;
  vector<int> result(count,0),left(count,0); queue<int> q;
  for(int x=0;x<n;++x)for(int y=0;y<n;++y)for(int z=0;z<n;++z)for(int t=0;t<2;++t){
   int s=encode(x,y,z,t);
   left[s]=t==0?adj[x].size():adj[y].size()*adj[z].size();
   if(x==y||x==z)result[s]=2;
   else if(phone[x])result[s]=1;
   if(result[s])q.push(s);
  }
  while(!q.empty()){
   int s=q.front();q.pop(); int v=s/2,t=s%2,z=v%n;v/=n;int y=v%n,x=v/n;
   int pt=1-t;
   if(pt==0){
    for(unsigned i=0;i<adj[x].size();++i){
     int p=encode(adj[x][i],y,z,pt); if(result[p])continue;
     if(result[s]==1||--left[p]==0){result[p]=result[s];q.push(p);}
    }
   }else{
    for(unsigned i=0;i<adj[y].size();++i)for(unsigned j=0;j<adj[z].size();++j){
     int p=encode(x,adj[y][i],adj[z][j],pt);if(result[p])continue;
     if(result[s]==2||--left[p]==0){result[p]=result[s];q.push(p);}
    }
   }
  }
  int r=result[encode(m,a,b,0)];
  cout<<(r==1?"You can escape.":r==2?"You are eliminated.":"You are trapped in the Matrix.")<<'\n';
 }
}
