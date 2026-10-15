// POJ 2806 - Janken Tactics
// Model: gpt-6.1-sol
// Dijkstra on the 61 hexagonal cells minimizes terrain entry costs.
// Enemy counters forbid intermediate cells only; the start can be left
// and the destination can be entered even inside an enemy threat zone.
// The input's 'six lines' is a typo: the board has nine terrain rows.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int terrain[9][10], side[9][10], type[9][10];
int dr[6]={0,0,-1,-1,1,1}, dc[6]={-1,1,0,1,0,-1};
bool valid(int r,int c){return r>=0&&r<9&&c>=max(1,5-r)&&c<=min(9,13-r);}
int kind(char c){return c=='G'?0:c=='M'?1:2;}
bool beats(int a,int b){return (a==0&&b==2)||(a==1&&b==0)||(a==2&&b==1);}
int main(){
 int n; cin>>n;
 for(int game=1;game<=n;++game){
  for(int r=0;r<9;++r)for(int c=max(1,5-r);c<=min(9,13-r);++c){
   char t;cin>>t; terrain[r][c]=t=='F'?1:t=='W'?2:t=='H'?3:t=='M'?4:999;side[r][c]=-1;
  }
  int counts[2];cin>>counts[0]>>counts[1];
  for(int s=0;s<2;++s)for(int i=0;i<counts[s];++i){char t;string p;cin>>t>>p;int r=p[0]-'A',c=p[1]-'0';side[r][c]=s;type[r][c]=kind(t);}
  int v;cin>>v;cout<<"Game #"<<game<<"\n";
  for(int move=1;move<=v;++move){
   string a,b;cin>>a>>b;int sr=a[0]-'A',sc=a[1]-'0',er=b[0]-'A',ec=b[1]-'0';int answer=-1;
   if(side[sr][sc]!=-1&&side[er][ec]==-1&&terrain[er][ec]<999){
    bool forbidden[9][10]={{false}},used[9][10]={{false}};int d[9][10];
    for(int r=0;r<9;++r)for(int c=1;c<=9;++c)d[r][c]=999;
    for(int r=0;r<9;++r)for(int c=1;c<=9;++c)if(valid(r,c)&&side[r][c]!=-1&&side[r][c]!=side[sr][sc]){
     forbidden[r][c]=true;
     if(beats(type[r][c],type[sr][sc]))for(int k=0;k<6;++k){int x=r+dr[k],y=c+dc[k];if(valid(x,y))forbidden[x][y]=true;}
    }
    forbidden[er][ec]=false;d[sr][sc]=0;
    for(int step=0;step<61;++step){
     int x=-1,y=-1,best=999;
     for(int r=0;r<9;++r)for(int c=1;c<=9;++c)if(valid(r,c)&&!used[r][c]&&d[r][c]<best){best=d[r][c];x=r;y=c;}
     if(x==-1||best>10)break;
     if(x==er&&y==ec){answer=10-best;break;}used[x][y]=true;
     for(int k=0;k<6;++k){int r=x+dr[k],c=y+dc[k];if(valid(r,c)&&!forbidden[r][c])d[r][c]=min(d[r][c],best+terrain[r][c]);}
    }
    if(answer>=0){side[er][ec]=side[sr][sc];type[er][ec]=type[sr][sc];side[sr][sc]=-1;}
   }
   cout<<"Move #"<<move<<" ("<<a<<" -> "<<b<<"): ";
   if(answer<0)cout<<"Unsuccessful\n";else cout<<"Successful ("<<answer<<" points left)\n";
  }
 }
}
