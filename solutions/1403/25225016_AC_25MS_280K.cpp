// POJ 1403 - Hotline
// Model: gpt-6.1-sol
// Keep facts in input order, normalize verbs, and detect any contradictory
// activity assertions. Generic subjects apply to every named subject.
// Answers swap I/you, deduplicate activities, and preserve first mention order.
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;
struct Fact { string s,v,o; bool yes; };
string word(string &s) { size_t p=s.find(' '); string w=s.substr(0,p); s=p==string::npos?"":s.substr(p+1); return w; }
bool generic(string s) { return s=="everybody" || s=="nobody"; }
string swapper(string s) { return s=="I"?"you":s=="you"?"I":s; }
bool plural(string s) { return s=="I" || s=="you"; }
string activity(const Fact &f, bool pl) { return (f.yes ? f.v+(pl?"":"s") : string(pl?"don't ":"doesn't ")+f.v)+(f.o.empty()?"":" "+f.o); }
bool same(const Fact &a,const Fact &b) { return a.v==b.v && a.o==b.o; }
string answer(string q,const vector<Fact>& fs) {
 string first=word(q);
 if(first=="do" || first=="does") {
  string s=word(q); Fact f; f.v=word(q); f.o=q;
  for(size_t i=0;i<fs.size();++i) if(same(f,fs[i]) && (fs[i].s==s || generic(fs[i].s))) {
   string out=swapper(s); return string(fs[i].yes?"yes, ":"no, ")+out+" "+activity(fs[i],plural(out))+".";
  }
  return "maybe.";
 }
 if(first=="who") {
  Fact f; f.v=word(q); f.v.erase(f.v.size()-1); f.o=q;
  vector<string> names; set<string> seen;
  for(size_t i=0;i<fs.size();++i) if(same(f,fs[i])) {
   if(generic(fs[i].s)) { Fact a=fs[i]; a.yes=true; return fs[i].s+" "+activity(a,false)+"."; }
   if(fs[i].yes && seen.insert(fs[i].s).second) names.push_back(swapper(fs[i].s));
  }
  if(names.empty()) return "I don't know.";
  string out;
  for(size_t i=0;i<names.size();++i) { if(i) out+=(i+1==names.size()?" and ":", "); out+=names[i]; }
  f.yes=true; return out+" "+activity(f,names.size()>1 || plural(names[0]))+".";
 }
 word(q); string s=word(q), out=swapper(s); vector<Fact> acts; set<pair<string,string> > seen;
 for(size_t i=0;i<fs.size();++i) if((fs[i].s==s || generic(fs[i].s)) && seen.insert(make_pair(fs[i].v,fs[i].o)).second) acts.push_back(fs[i]);
 if(acts.empty()) return "I don't know.";
 for(size_t i=0;i<acts.size();++i) { out+=(i==0?" ":i+1==acts.size()?", and ":", "); out+=activity(acts[i],plural(s)); }
 return out+".";
}
int main() {
 int t; cin>>t; string line; getline(cin,line);
 for(int d=1;d<=t;++d) {
  vector<Fact> fs; bool bad=false; cout<<"Dialogue #"<<d<<":\n";
  while(getline(cin,line)) {
   if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
   if(line.empty()) continue;
   char end=line[line.size()-1];
   if(end=='!') { cout<<line<<"\n\n"; break; }
   string text=line.substr(0,line.size()-1);
   if(end=='?') { cout<<line<<"\n"<<(bad?"I am abroad.":answer(text,fs))<<"\n\n"; continue; }
   Fact f; f.s=word(text); f.v=word(text); f.yes=true;
   if(f.v=="don't" || f.v=="doesn't") { f.yes=false; f.v=word(text); }
   else { if(!plural(f.s)) f.v.erase(f.v.size()-1); if(f.s=="nobody") f.yes=false; }
   f.o=text;
   for(size_t i=0;i<fs.size();++i) if(same(f,fs[i]) && f.yes!=fs[i].yes && (f.s==fs[i].s || generic(f.s) || generic(fs[i].s))) bad=true;
   fs.push_back(f);
  }
 }
}
