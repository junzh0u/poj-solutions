// POJ 2770 - Tantrix
// Model: gpt-6.1-sol
// Enumerate adjacent empty cells, five tiles, and all six rotations.
// Controlled sides extend along supported straight boundaries from forced
// spaces, as shown in the figure and confirmed by the contest reference.
// Reject mismatched edges and new empty spaces with three equal links;
// count forced placements first, falling back only if none are legal.
#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;
typedef pair<int,int> P;
int dx[6]={0,1,1,0,-1,-1},dy[6]={1,0,-1,-1,0,1};
P step(P p,int d){return P(p.first+dx[d],p.second+dy[d]);}
map<P,string> b;
set<P> forced;
bool occupied(P p){return b.find(p)!=b.end();}
int degree(P p){int n=0;for(int d=0;d<6;++d)n+=occupied(step(p,d));return n;}
bool controlled(P p){
 for(int d=0;d<6;++d){
  P q=step(p,d);
  while(!occupied(q) && (occupied(step(q,(d+2)%6)) || occupied(step(q,(d+4)%6)))){
   if(forced.count(q))return true;
   q=step(q,d);
  }
 }
 return false;
}
bool legal(P p,const string &s){
 for(int d=0;d<6;++d){
  P q=step(p,d);
  if(occupied(q)){
   if(b[q][(d+3)%6]!=s[d])return false;
  }else{
   int same=1;
   for(int k=0;k<6;++k){
    map<P,string>::iterator it=b.find(step(q,k));
    if(it!=b.end() && it->second[(k+3)%6]==s[d])++same;
   }
   if(same>=3)return false;
  }
 }
 return true;
}
int main(){
 int t;cin>>t;while(t--){
  b.clear();forced.clear();set<P> cells;int n;cin>>n;
  for(int i=0;i<n;++i){int x,y;string s;cin>>x>>y>>s;b[P(x,y)]=s;}
  for(map<P,string>::iterator it=b.begin();it!=b.end();++it)
   for(int d=0;d<6;++d){P q=step(it->first,d);if(!occupied(q))cells.insert(q);}
  for(set<P>::iterator it=cells.begin();it!=cells.end();++it)if(degree(*it)>=3)forced.insert(*it);
  set<P> allowed;for(set<P>::iterator it=cells.begin();it!=cells.end();++it)if(!controlled(*it))allowed.insert(*it);
  int ans[2]={0,0};
  for(int i=0;i<5;++i){string s;cin>>s;for(int r=0;r<6;++r){
   for(set<P>::iterator it=allowed.begin();it!=allowed.end();++it)if(legal(*it,s))++ans[forced.count(*it)?1:0];
   s=s.substr(1)+s[0];
  }}
  cout<<(ans[1]?ans[1]:ans[0])<<'\n';
 }
}
