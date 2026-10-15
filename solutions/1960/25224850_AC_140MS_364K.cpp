// POJ 1960 - Time Planner
// Model: gpt-6.1-sol
// Sweep appointment endpoints, counting distinct busy members. All equal-time
// events are processed together; join adjacent feasible spans into maximal
// meetings. Every month has exactly 30 days, including February.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
using namespace std;
typedef long long Time;
struct Event {
    Time t; int member, delta;
    Event(Time a,int b,int c):t(a),member(b),delta(c) {}
    bool operator<(const Event& e) const { return t<e.t; }
};
Time readtime() {
    int y,m,d,h,n,s; cin>>y>>m>>d>>h>>n>>s;
    return (((Time(y-1800)*360+(m-1)*30+d-1)*24+h)*60+n)*60+s;
}
void printtime(Time t) {
    int s=int(t%60); t/=60; int n=int(t%60); t/=60;
    int h=int(t%24); t/=24; int d=int(t%30)+1; t/=30;
    int m=int(t%12)+1; int y=int(t/12)+1800;
    char b[64]; sprintf(b,"%02d/%02d/%04d %02d:%02d:%02d",m,d,y,h,n,s);
    cout<<b;
}
void meeting(Time a,Time b) {
    cout<<"appointment possible from "; printtime(a); cout<<" to "; printtime(b); cout<<'\n';
}
int main() {
    int cases; cin>>cases;
    const Time end=Time(400)*360*24*60*60;
    for(int cs=1;cs<=cases;++cs) {
        int m; cin>>m; vector<Event> e;
        for(int i=0;i<m;++i) {
            int n; cin>>n;
            for(int j=0;j<n;++j) {
                Time a=readtime(),b=readtime(); string text; getline(cin,text);
                if(a<b) { e.push_back(Event(a,i,1)); e.push_back(Event(b,i,-1)); }
            }
        }
        e.push_back(Event(0,m,0)); e.push_back(Event(end,m,0)); sort(e.begin(),e.end());
        int count[21]={0},busy=0; Time start=-1; bool printed=false;
        cout<<"Scenario #"<<cs<<":\n";
        for(size_t i=0;i<e.size();) {
            Time t=e[i].t;
            while(i<e.size() && e[i].t==t) {
                int id=e[i].member; bool was=count[id]>0;
                count[id]+=e[i].delta; bool now=count[id]>0;
                busy+=int(now)-int(was); ++i;
            }
            bool good=(busy<=1 && m-busy>=2 && t<end);
            if(good && start<0) start=t;
            if(!good && start>=0) {
                if(t-start>=3600) { meeting(start,t); printed=true; }
                start=-1;
            }
        }
        if(!printed) cout<<"no appointment possible\n";
        cout<<'\n';
    }
}
