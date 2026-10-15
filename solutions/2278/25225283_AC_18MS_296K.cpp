// POJ 2278 - Histology Assistant
// Model: gpt-6.1-sol
// Label four-connected components, then follow the clockwise Moore contour.
// Restrict every contour to its own component; diagonal contact does not merge
// components. Stop at the first directed edge, not merely the start pixel,
// because a thin branch can revisit that pixel before the contour is complete.
// A diagonal step must have its clockwise cardinal neighbor in the component;
// this prevents jumping across a corner into a hole.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
using namespace std;
int a[50][66], lab[50][66];
int dr[8]={-1,-1,0,1,1,1,0,-1};
int dc[8]={0,1,1,1,0,-1,-1,-1};
struct Result { int r,c; string s; };
int main() {
 int n,m,k;
 while(cin>>n>>m>>k && n) {
  for(int i=0;i<50;i++) for(int j=0;j<66;j++) a[i][j]=lab[i][j]=0;
  for(int i=1;i<=n;i++) { string s; cin>>s; for(int j=1;j<=m;j++) a[i][j]=(s[j-1]=='X'); }
  vector<Result> out; int id=0;
  for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) if(a[i][j]&&!lab[i][j]) {
   ++id; queue<pair<int,int> > q; q.push(make_pair(i,j)); lab[i][j]=id; int count=0;
   while(!q.empty()) {
    int r=q.front().first,c=q.front().second; q.pop(); ++count;
    for(int d=0;d<8;d+=2) { int r2=r+dr[d],c2=c+dc[d];
     if(a[r2][c2]&&!lab[r2][c2]) {lab[r2][c2]=id;q.push(make_pair(r2,c2));}
    }
   }
   if(count<k) continue;
   Result res; res.r=i;res.c=j;
   int r=i,c=j,back=6,first=-1;
   while(true) {
    int d=(back+1)%8;
    while(lab[r+dr[d]][c+dc[d]]!=id ||
          ((d%2) && lab[r+dr[(d+1)%8]][c+dc[(d+1)%8]]!=id)) d=(d+1)%8;
    if(first<0) first=d;
    else if(r==i&&c==j&&d==first) break;
    res.s+=char('A'+d);
    // The last empty neighbor scanned becomes the new backtrack pixel.
    int prev=(d+7)%8;
    int br=r+dr[prev],bc=c+dc[prev];
    r+=dr[d];c+=dc[d];
    for(back=0;back<8;back++) if(r+dr[back]==br&&c+dc[back]==bc) break;
   }
   out.push_back(res);
  }
  cout<<out.size()<<'\n';
  for(size_t i=0;i<out.size();i++) {
   cout<<out[i].r<<' '<<out[i].c<<' '<<out[i].s.size()<<'\n';
   for(size_t p=0;p<out[i].s.size();p+=40) cout<<out[i].s.substr(p,40)<<'\n';
  }
 }
}
