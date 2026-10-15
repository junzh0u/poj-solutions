// POJ 2293 - Hey, Pay Day!
// Model: gpt-6.1-sol
// Each lone record chooses one of the worker's two watchman events.
// Inverted timestamps forbid pairs of choices, giving a 2-SAT instance.
// Its implication closure tests extra event-time bounds as unit clauses.
// Days are independent; sum each worker's daily minimum and maximum.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
struct Day { vector<string> events; map<string, vector<int> > records; };
struct Anchor { int pos,time,lit; Anchor(int p,int t,int l):pos(p),time(t),lit(l){} };
struct Solver {
 int n; unsigned long reach[30], forced; vector<Anchor> a;
 void unit(int l) { forced |= 1UL<<l; }
 void forbid(int x,int y) {
  if(x<0) { if(y>=0) unit(y^1); }
  else if(y<0) unit(x^1);
  else {reach[x]|=1UL<<(y^1); reach[y]|=1UL<<(x^1);}
 }
 bool possible(int lit,int p,int x,int mode) {
  unsigned long mask=forced | (1UL<<lit), closure=0;
  for(size_t i=0;i<a.size();++i) {
   bool bad=(mode==0 && a[i].pos<p && a[i].time>x) ||
            (mode==1 && a[i].pos>p && a[i].time<x);
   if(bad) {if(a[i].lit<0) return false; mask|=1UL<<(a[i].lit^1);}
  }
  for(int i=0;i<n;++i) if(mask&(1UL<<i)) closure|=reach[i];
  for(int i=0;i<n;i+=2) if((closure&(3UL<<i))==(3UL<<i)) return false;
  return true;
 }
};
int seconds(const string& s) {int h,m,t; char c; istringstream in(s); in>>h>>c>>m>>c>>t; return h*3600+m*60+t;}
void duration(int t) {cout<<t/3600<<' '<<t/60%60<<' '<<t%60;}
int main(){
 int tc; string line; cin>>tc; getline(cin,line);
 while(tc--){
  map<string,Day> days; map<string,pair<int,int> > total;
  while(getline(cin,line) && line!="#") {
   if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
   if(line=="#") break; if(line.empty()) continue;
   string date=line.substr(0,8);
   if(line[8]=='-') {
    size_t dash=line.find('-',9); string name=line.substr(9,dash-9);
    days[date].records[name].push_back(seconds(line.substr(dash+1)));
    total[name]=make_pair(0,0);
   } else {istringstream in(line.substr(9)); string name; while(in>>name) days[date].events.push_back(name);}
  }
  for(map<string,Day>::iterator di=days.begin();di!=days.end();++di){
   Day &d=di->second; map<string,vector<int> > pos;
   for(size_t i=0;i<d.events.size();++i) pos[d.events[i]].push_back((int)i);
   Solver s; s.n=0; s.forced=0; map<string,int> var;
   for(map<string,vector<int> >::iterator r=d.records.begin();r!=d.records.end();++r){
    vector<int>& t=r->second; sort(t.begin(),t.end()); vector<int>& p=pos[r->first];
    if(t.size()==2) {s.a.push_back(Anchor(p[0],t[0],-1));s.a.push_back(Anchor(p[1],t[1],-1));}
    else {var[r->first]=s.n; s.a.push_back(Anchor(p[0],t[0],s.n++));s.a.push_back(Anchor(p[1],t[0],s.n++));}
   }
   for(int i=0;i<s.n;++i) s.reach[i]=1UL<<i;
   for(size_t i=0;i<s.a.size();++i) for(size_t j=0;j<i;++j)
    if((s.a[i].pos<s.a[j].pos && s.a[i].time>s.a[j].time) || (s.a[i].pos>s.a[j].pos && s.a[i].time<s.a[j].time)) s.forbid(s.a[i].lit,s.a[j].lit);
   for(int k=0;k<s.n;++k) for(int i=0;i<s.n;++i) if(s.reach[i]&(1UL<<k)) s.reach[i]|=s.reach[k];
   for(map<string,vector<int> >::iterator r=d.records.begin();r!=d.records.end();++r){
    int mn=1000000,mx=-1; vector<int>& t=r->second;
    if(t.size()==2) mn=mx=t[1]-t[0];
    else for(int c=0;c<2;++c){
     int lit=var[r->first]+c,p=pos[r->first][1-c];
     if(!s.possible(lit,p,72000,0) || !s.possible(lit,p,28800,1)) continue;
     int l=28800,h=72000;
     while(l<h){int m=(l+h)/2;if(s.possible(lit,p,m,0)) h=m;else l=m+1;} int low=l;
     l=28800;h=72000;
     while(l<h){int m=(l+h+1)/2;if(s.possible(lit,p,m,1)) l=m;else h=m-1;} int high=l;
     int small=c==0?low-t[0]:t[0]-high;
     int large=c==0?high-t[0]:t[0]-low;
     mn=min(mn,small);mx=max(mx,large);
    }
    total[r->first].first+=mn;total[r->first].second+=mx;
   }
  }
  for(map<string,pair<int,int> >::iterator i=total.begin();i!=total.end();++i){cout<<i->first<<'-';duration(i->second.first);cout<<'-';duration(i->second.second);cout<<'\n';}cout<<"#\n";
 }
}
