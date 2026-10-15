// POJ 1253 - Markov Trains
// Model: gpt-6.1-sol
// Enumerate fixed station routes, propagating their arrival-time distributions.
// Every available service is tried in departure order, even if it arrives too
// late: a running slow train cannot be skipped. Repeated stations are allowed.
// Surviving probability bounds each continuation; reverse temporal reachability
// removes arrival events that cannot possibly reach the destination in time.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Train { int dep, arr; double cancel; };
struct Event { int ready; double prob; Event(int t,double p):ready(t),prob(p){} };
bool trainLess(const Train&a,const Train&b){return a.dep<b.dep;}
bool eventLess(const Event&a,const Event&b){return a.ready<b.ready;}
vector<Train> trains[12][12];
int latest[12], goal;
double best;
vector<int> path, answer;
struct Candidate { int station; double mass; vector<Event> events; };
bool candidateLess(const Candidate&a,const Candidate&b){return a.mass>b.mass;}
void search(int station,const vector<Event>& events,double mass) {
    if(mass<=best) return;
    if(station==goal){best=mass;answer=path;return;}
    vector<Candidate> candidates;
    for(int v=0;v<12;++v){
        if(trains[station][v].empty())continue;
        Candidate c;c.station=v;c.mass=0;
        double waiting=0;unsigned k=0;
        const vector<Train>& ts=trains[station][v];
        for(unsigned j=0;j<ts.size();++j){
            while(k<events.size() && events[k].ready<=ts[j].dep)
                waiting+=events[k++].prob;
            double take=waiting*(1-ts[j].cancel);
            if(take>0 && ts[j].arr+1<=latest[v])
                c.events.push_back(Event(ts[j].arr+1,take));
            waiting*=ts[j].cancel;
        }
        sort(c.events.begin(),c.events.end(),eventLess);
        vector<Event> merged;
        for(unsigned j=0;j<c.events.size();++j){
            c.mass+=c.events[j].prob;
            if(!merged.empty() && merged.back().ready==c.events[j].ready)
                merged.back().prob+=c.events[j].prob;
            else merged.push_back(c.events[j]);
        }
        c.events.swap(merged);
        if(c.mass>best)candidates.push_back(c);
    }
    sort(candidates.begin(),candidates.end(),candidateLess);
    for(unsigned i=0;i<candidates.size();++i){
        path.push_back(candidates[i].station);
        search(candidates[i].station,candidates[i].events,candidates[i].mass);
        path.pop_back();
    }
}
int readTime(){int h,m;scanf("%d:%d",&h,&m);return h*60+m;}
int main(){
    int runs;scanf("%d",&runs);
    while(runs--){
        for(int i=0;i<12;++i)for(int j=0;j<12;++j)trains[i][j].clear();
        int n;scanf("%d",&n);
        for(int i=0;i<n;++i){
            char x,y;Train t;scanf(" %c",&x);t.dep=readTime();
            scanf(" %c",&y);t.arr=readTime();scanf("%lf",&t.cancel);
            trains[x-'A'][y-'A'].push_back(t);
        }
        char a,b;scanf(" %c",&a);int start=readTime();
        scanf(" %c",&b);int deadline=readTime();goal=b-'A';
        for(int i=0;i<12;++i){latest[i]=-1;for(int j=0;j<12;++j)
            sort(trains[i][j].begin(),trains[i][j].end(),trainLess);}
        latest[goal]=deadline+1;
        for(int pass=0;pass<n;++pass){
            bool changed=false;
            for(int i=0;i<12;++i)for(int j=0;j<12;++j)
                for(unsigned k=0;k<trains[i][j].size();++k){
                    Train t=trains[i][j][k];
                    if(t.arr+1<=latest[j] && t.dep>latest[i]){
                        latest[i]=t.dep;changed=true;
                    }
                }
            if(!changed)break;
        }
        best=0;path.clear();answer.clear();path.push_back(a-'A');
        vector<Event> initial;initial.push_back(Event(start,1));
        search(a-'A',initial,1);
        for(unsigned i=0;i<answer.size();++i)printf("%s%c",i?" ":"",'A'+answer[i]);
        printf("\n%.4f\n",best);
    }
    return 0;
}
