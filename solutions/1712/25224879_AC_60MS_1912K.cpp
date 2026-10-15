// POJ 1712 - Flying Stars
// Model: gpt-6.1-sol
// Earliest-arrival Dijkstra on daily flight schedules. Earlier arrival can
// always wait for the same flight, so one label per airport suffices.
// Boarding is required at the origin and each transfer, not at destination.
// A negative time-zone sign applies to both hours and minutes.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
struct Flight { string name; int to, departure, duration; };
struct Airport { int zone, boarding; vector<Flight> flights; };
int minutes(const string &s) { return ((s[0]-'0')*10+s[1]-'0')*60+(s[3]-'0')*10+s[4]-'0'; }
int modday(int x) { x%=1440; return x<0?x+1440:x; }
map<string,int> ids;
int identify(const string &s) {
    if(ids.find(s)==ids.end()) { int k=ids.size(); ids[s]=k; }
    return ids[s];
}
int main() {
    string origin,destination,start;
    if(!(cin>>origin>>destination>>start)) return 0;
    int src=identify(origin), dst=identify(destination), n; cin>>n;
    vector<Airport> airports(n);
    for(int i=0;i<n;++i) {
        string name,zone,boarding; int m;
        cin>>name>>zone>>boarding>>m;
        int u=identify(name);
        airports[u].zone=minutes(zone.substr(1))*(zone[0]=='-'?-1:1);
        airports[u].boarding=minutes(boarding);
        for(int j=0;j<m;++j) {
            Flight f; string target,dep,duration;
            cin>>f.name>>target>>dep>>duration;
            f.to=identify(target); f.departure=minutes(dep); f.duration=minutes(duration);
            airports[u].flights.push_back(f);
        }
    }
    int startUTC=minutes(start)-airports[src].zone;
    vector<int> dist(n,1000000000), used(n,0), prev(n,-1);
    vector<string> route(n);
    dist[src]=0;
    for(int i=0;i<n;++i) {
        int u=-1;
        for(int j=0;j<n;++j) if(!used[j] && (u==-1 || dist[j]<dist[u])) u=j;
        if(u==-1 || dist[u]==1000000000 || u==dst) break;
        used[u]=1;
        int ready=dist[u]+airports[u].boarding;
        int local=modday(startUTC+ready+airports[u].zone);
        for(size_t j=0;j<airports[u].flights.size();++j) {
            const Flight &f=airports[u].flights[j];
            int arrival=ready+modday(f.departure-local)+f.duration;
            if(arrival<dist[f.to]) { dist[f.to]=arrival; prev[f.to]=u; route[f.to]=f.name; }
        }
    }
    int total=dist[dst], local=modday(startUTC+total+airports[dst].zone);
    cout<<total/1440<<':'<<setfill('0')<<setw(2)<<total/60%24<<':'<<setw(2)<<total%60<<'\n';
    cout<<setw(2)<<local/60<<':'<<setw(2)<<local%60<<'\n';
    vector<string> path;
    for(int v=dst;v!=src;v=prev[v]) path.push_back(route[v]);
    reverse(path.begin(),path.end());
    for(size_t j=0;j<path.size();++j) cout<<path[j]<<'\n';
    return 0;
}
