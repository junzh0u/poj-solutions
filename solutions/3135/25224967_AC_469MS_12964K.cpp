// POJ 3135 - Polygons on the Grid
// Model: gpt-6.1-sol
// Enumerate partitions into two chains of at most three lattice vectors.
// Store the largest signed double area for each endpoint, then join opposite
// endpoints. All orders are covered; a maximum-area closed vector ordering is
// convex (uncrossing / exchanging reversed turns only increases its area).
// Collinear consecutive rods are allowed, as in the six equal rods sample.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct P { int x,y; P(int a=0,int b=0):x(a),y(b){} };
int cross(P a,P b){return a.x*b.y-a.y*b.x;}
vector<P> dirs[301];
const int W=1801, OFF=900, NEG=-1000000000;
int best[W*W];
vector<int> group;
P v[3];
int answer;
bool storing;
int chainArea(int k){
 if(k==1) return 0;
 if(k==2) return abs(cross(v[0],v[1]));
 int a=cross(v[0],v[1]), b=cross(v[0],v[2]), c=cross(v[1],v[2]);
 return max(abs(a+b+c),max(abs(a+b-c),abs(-a+b+c)));
}
void generate(int dep,int x,int y){
 if(dep==(int)group.size()){
  int area=chainArea(dep);
  if(storing){ int &z=best[(x+OFF)*W+y+OFF]; z=max(z,area); }
  else {int z=best[(OFF-x)*W+OFF-y]; if(z!=NEG) answer=max(answer,z+area);}
  return;
 }
 const vector<P>& d=dirs[group[dep]];
 for(int i=0;i<(int)d.size();++i){v[dep]=d[i];generate(dep+1,x+d[i].x,y+d[i].y);}
}
int main(){
 for(int r=1;r<=300;++r) for(int x=-r;x<=r;++x){
  int y=(int)(sqrt(double(r*r-x*x))+0.5);
  if(x*x+y*y==r*r){dirs[r].push_back(P(x,y));if(y)dirs[r].push_back(P(x,-y));}
 }
 int n;
 while(cin>>n && n){
  int r[6];for(int i=0;i<n;++i)cin>>r[i];answer=0;
  int k=n/2;
  for(int mask=1;mask<(1<<n);++mask){
   if(!(mask&1))continue;
   int cnt=0;for(int i=0;i<n;++i)if(mask&(1<<i))++cnt;
   if(cnt!=k)continue;
   fill(best,best+W*W,NEG);
   group.clear();for(int i=0;i<n;++i)if(mask&(1<<i))group.push_back(r[i]);
   storing=true;generate(0,0,0);
   group.clear();for(int i=0;i<n;++i)if(!(mask&(1<<i)))group.push_back(r[i]);
   storing=false;generate(0,0,0);
  }
  cout<<(answer?answer/2:-1)<<'\n';
 }
}
