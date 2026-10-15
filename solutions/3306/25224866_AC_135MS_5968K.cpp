// POJ 3306 - Busy Airport
// Model: gpt-6.1-sol
// Simulate requests by timestamp and ID, freeing every completed runway first.
// Sort the resulting report separately so landings precede requests on ties.
// Each input record owns its duration: aircraft IDs may occur more than once.
// Dates use the statement's Persian-calendar leap rule, including beyond 1633.
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
const int DAY=86400000;
int ys[3001];
bool leap(int y) { int r=y%33; return r==1||r==5||r==9||r==13||r==17||r==22||r==30; }
int monthlen(int y,int m) { return m<=6?31:(m<=11?30:29+leap(y)); }
struct Stamp {
 int day,ms;
 Stamp(int d=0,int t=0):day(d),ms(t){}
 bool operator<(const Stamp& b) const { return day!=b.day?day<b.day:ms<b.ms; }
 Stamp add(int t) const { return Stamp(day+(ms+t)/DAY,(ms+t)%DAY); }
};
struct Request { Stamp t; int id,dur,seq; };
struct Later {
 bool operator()(const Request&a,const Request&b) const {
  if(a.t<b.t) return false; if(b.t<a.t) return true;
  return a.id!=b.id?a.id>b.id:a.seq>b.seq;
 }
};
struct EndLater { bool operator()(const Stamp&a,const Stamp&b) const {return b<a;} };
struct Entry { Stamp t; int id,kind,seq; Entry(Stamp a,int b,int c,int d):t(a),id(b),kind(c),seq(d){} };
bool before(const Entry&a,const Entry&b) {
 if(a.t<b.t) return true; if(b.t<a.t) return false;
 if((a.kind==0)!=(b.kind==0)) return a.kind==0;
 if(a.id!=b.id) return a.id<b.id;
 return a.seq<b.seq;
}
int readtime() {int h,m,s,u; scanf("%d:%d:%d.%d",&h,&m,&s,&u); return ((h*60+m)*60+s)*1000+u;}
int main() {
 for(int y=0;y<3000;++y) ys[y+1]=ys[y]+365+leap(y);
 int T; scanf("%d",&T);
 for(int tc=1;tc<=T;++tc) {
  int R,B; scanf("%d%d",&R,&B);
  priority_queue<Request,vector<Request>,Later> requests;
  priority_queue<Stamp,vector<Stamp>,EndLater> occupied;
  vector<Entry> report;
  for(int i=0;i<R;++i) {
   Request a; int d,m,y; scanf("%d%d/%d/%d",&a.id,&d,&m,&y);
   int day=ys[y]+d-1; for(int j=1;j<m;++j) day+=monthlen(y,j);
   a.t=Stamp(day,readtime()); a.dur=readtime(); a.seq=i; requests.push(a);
  }
  int seq=0;
  while(!requests.empty()) {
   Request a=requests.top(); requests.pop();
   while(!occupied.empty() && !(a.t<occupied.top())) occupied.pop();
   if((int)occupied.size()<B) {
    report.push_back(Entry(a.t,a.id,1,seq++));
    Stamp end=a.t.add(a.dur); occupied.push(end);
    report.push_back(Entry(end,a.id,0,seq++));
   } else {
    report.push_back(Entry(a.t,a.id,2,seq++));
    a.t=a.t.add(600000); requests.push(a);
   }
  }
  sort(report.begin(),report.end(),before);
  printf("Report for Test-Case #%d:\n",tc);
  const char* names[]={"LANDED","ACCEPTED","POSTPONED"};
  for(size_t i=0;i<report.size();++i) {
   Entry e=report[i]; int y=int(upper_bound(ys,ys+3001,e.t.day)-ys)-1;
   int d=e.t.day-ys[y],m=1; while(d>=monthlen(y,m)) {d-=monthlen(y,m);++m;}
   int t=e.t.ms;
   printf("%d %02d/%02d/%04d %02d:%02d:%02d.%03d %s\n",e.id,d+1,m,y,t/3600000,t/60000%60,t/1000%60,t%1000,names[e.kind]);
  }
  printf("\n");
 }
 return 0;
}
