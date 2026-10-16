// POJ 1630 - Max Separation
// Model: gpt-6.1-sol
// Move a separating line to a supporting point of S0, then sweep its normal.
// Count sets wholly on the positive side while S0 has no positive point.
// Test open angular intervals: strict separation persists under perturbation.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
using namespace std;
struct Point { double x,y; Point(double a=0,double b=0):x(a),y(b){} };
struct Event { double a; int g,d; Event(double b,int c,int e):a(b),g(c),d(e){} bool operator<(const Event& o)const{return a<o.a;} };
int main(){
 int n; if(!(cin>>n))return 0;
 vector<vector<Point> > s(n+1);
 for(int g=0;g<=n;++g){
  string line; getline(cin,line,'#'); const char* p=line.c_str();
  while(*p){ while(*p && !(*p=='-' || (*p>='0' && *p<='9')))++p; if(!*p)break;
   char* q; double x=strtod(p,&q); p=q; while(*p && *p!=',')++p; if(!*p)break; ++p;
   double y=strtod(p,&q); p=q; s[g].push_back(Point(x,y));
  }
 }
 const double pi=acos(-1.0), tw=2*pi; int ans=0;
 for(size_t k=0;k<s[0].size();++k){
  Point p=s[0][k]; vector<Event> ev; vector<int> cnt(n+1,0);
  for(int g=0;g<=n;++g)for(size_t j=0;j<s[g].size();++j){
   double x=s[g][j].x-p.x,y=s[g][j].y-p.y; if(x==0 && y==0)continue;
   double a=atan2(y,x)+pi/2; if(a<0)a+=tw; if(a>=tw)a-=tw;
   ev.push_back(Event(a,g,-1)); a+=pi; if(a>=tw)a-=tw; ev.push_back(Event(a,g,1));
  }
  if(ev.empty())continue; sort(ev.begin(),ev.end());
  double a=(ev.back().a+ev.front().a+tw)/2; double nx=cos(a),ny=sin(a);
  for(int g=0;g<=n;++g)for(size_t j=0;j<s[g].size();++j)
   if(nx*(s[g][j].x-p.x)+ny*(s[g][j].y-p.y)>0)++cnt[g];
  int good=0; for(int g=1;g<=n;++g)if(cnt[g]==(int)s[g].size())++good;
  if(cnt[0]==0)ans=max(ans,good);
  for(size_t i=0;i<ev.size();){
   size_t j=i; while(j<ev.size() && ev[j].a-ev[i].a<1e-12){
    int g=ev[j].g; if(g && cnt[g]==(int)s[g].size())--good;
    cnt[g]+=ev[j].d; if(g && cnt[g]==(int)s[g].size())++good; ++j;
   }
   if(cnt[0]==0)ans=max(ans,good); i=j;
  }
 }
 cout<<ans<<'\n'; return 0;
}
