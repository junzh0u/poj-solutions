// POJ 1483 - Going in Circles on Alpha Centauri
// Model: gpt-6.1-sol
// Simulate arrivals and completions together. Pending requests are grouped by
// the smallest robot capacity that can carry them; each group is FIFO.
// Select the oldest feasible head, then the closest eligible idle robot.
// Busy time includes travel to the origin, loading, delivery, and unloading.
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long Time;
struct Request { Time t; int o,d,w; };
int main() {
    int n,m,cs=0;
    while(cin>>n>>m && n) {
        vector<int> load(m),cap;
        for(int i=0;i<m;++i) {cin>>load[i]; cap.push_back(load[i]);}
        sort(cap.begin(),cap.end()); cap.erase(unique(cap.begin(),cap.end()),cap.end());
        vector<Request> req; Request r;
        while(cin>>r.t>>r.o>>r.d>>r.w && r.t!=-1) req.push_back(r);
        vector< queue<int> > pending(cap.size());
        vector<Time> freeAt(m,0); vector<int> pos(m,1);
        Time now=req[0].t,finish=now; double wait=0,busy=0;
        int next=0,done=0;
        while(done<(int)req.size()) {
            while(next<(int)req.size() && req[next].t<=now) {
                int g=lower_bound(cap.begin(),cap.end(),req[next].w)-cap.begin();
                pending[g].push(next++);
            }
            for(;;) {
                int maxLoad=0;
                for(int i=0;i<m;++i) if(freeAt[i]<=now) maxLoad=max(maxLoad,load[i]);
                int id=-1,group=-1;
                for(int g=0;g<(int)cap.size() && cap[g]<=maxLoad;++g)
                    if(!pending[g].empty() && (id<0 || pending[g].front()<id)) {
                        id=pending[g].front(); group=g;
                    }
                if(id<0) break;
                r=req[id]; int robot=-1,dist=n+1;
                for(int i=0;i<m;++i) if(freeAt[i]<=now && load[i]>=r.w) {
                    int travel=(r.o-pos[i]+n)%n;
                    if(travel<dist) {dist=travel;robot=i;}
                }
                int duration=dist+10+(r.d-r.o+n)%n;
                freeAt[robot]=now+duration; pos[robot]=r.d;
                wait+=double(freeAt[robot]-r.t); busy+=duration;
                finish=max(finish,freeAt[robot]); pending[group].pop(); ++done;
            }
            if(done==(int)req.size()) break;
            Time event=-1;
            if(next<(int)req.size()) event=req[next].t;
            for(int i=0;i<m;++i) if(freeAt[i]>now && (event<0 || freeAt[i]<event)) event=freeAt[i];
            now=event;
        }
        cout<<"Simulation "<<++cs<<"\n"<<fixed<<setprecision(3)
            <<"Average wait time   = "<<wait/req.size()<<" minutes\n"
            <<"Average utilization = "<<100.0*busy/(m*double(finish-req[0].t))<<" %\n\n";
    }
}
