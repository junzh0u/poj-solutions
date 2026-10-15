// POJ 1913 - Cocktails
// Model: gpt-6.1-sol
// Simulate circular recipe selection, deleting permanently exhausted recipes.
// The last successfully mixed cocktail is ineligible, even after wrapping;
// ignored orders do not change it. Prices use 64-bit integer arithmetic.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
struct Recipe { string name; vector<pair<int,int> > need; int count; long long price; };
vector<Recipe> r;
bool ranker(int a,int b) { return r[a].count!=r[b].count?r[a].count>r[b].count:a<b; }
int ingredient(map<string,int>& ids,const string& s) {
 map<string,int>::iterator it=ids.find(s); if(it!=ids.end()) return it->second;
 int id=ids.size(); ids[s]=id; return id;
}
int main() {
 ios::sync_with_stdio(false); cin.tie(0);
 int cases; cin>>cases;
 for(int tc=1;tc<=cases;++tc) {
  int n; cin>>n; r.assign(n,Recipe()); map<string,int> cocktails,ids;
  for(int i=0;i<n;++i) {
   int k; cin>>r[i].name>>k; cocktails[r[i].name]=i;
   for(int j=0;j<k;++j) { string s; int amount; cin>>s>>amount; r[i].need.push_back(make_pair(ingredient(ids,s),amount)); }
  }
  int m; cin>>m; vector<int> stock(ids.size(),0),cost(ids.size(),0);
  for(int i=0;i<m;++i) {
   string s; int a,p; cin>>s>>a>>p; int id=ingredient(ids,s);
   if(id>=(int)stock.size()) {stock.resize(id+1,0); cost.resize(id+1,0);}
   stock[id]=a; cost[id]=p;
  }
  set<int> active;
  for(int i=0;i<n;++i) {
   r[i].price=10000; active.insert(i);
   for(size_t j=0;j<r[i].need.size();++j) r[i].price+=3LL*r[i].need[j].second*cost[r[i].need[j].first];
  }
  int q,last=-1; cin>>q;
  while(q--) {
   string s; cin>>s; map<string,int>::iterator order=cocktails.find(s);
   if(order==cocktails.end()||active.empty()) continue;
   int start=order->second,selected=-1;
   set<int>::iterator it=active.lower_bound(start); if(it==active.end()) it=active.begin();
   int remaining=active.size();
   while(remaining--) {
    int x=*it; ++it; if(it==active.end()) it=active.begin();
    if(x==last) continue;
    bool ok=true;
    for(size_t j=0;j<r[x].need.size();++j) if(stock[r[x].need[j].first]<r[x].need[j].second) {ok=false;break;}
    if(ok) {selected=x;break;}
    active.erase(x); if(active.empty()) break;
   }
   if(selected!=-1) {
    for(size_t j=0;j<r[selected].need.size();++j) stock[r[selected].need[j].first]-=r[selected].need[j].second;
    ++r[selected].count; last=selected;
   }
  }
  vector<int> ranking; for(int i=0;i<n;++i) if(r[i].count) ranking.push_back(i);
  sort(ranking.begin(),ranking.end(),ranker);
  cout<<"Scenario "<<tc<<" top cocktails:\n";
  for(size_t i=0;i<ranking.size()&&i<10;++i) {int x=ranking[i];cout<<i+1<<" "<<r[x].name<<" "<<r[x].count<<" "<<r[x].price<<"\n";}
 }
}
