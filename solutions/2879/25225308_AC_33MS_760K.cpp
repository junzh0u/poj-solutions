// POJ 2879 - Vice City
// Model: gpt-6.1-sol
// Dijkstra on (location, current vehicle). Roads retain the current vehicle;
// replacing it at a location costs one minute. A slower replacement is useless.
// Start with the PayPhone vehicle for free; output truncates, rather than rounds.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <cstdio>
#include <cmath>
using namespace std;
struct State {
    double time; int place, car;
    State(double t,int p,int c):time(t),place(p),car(c){}
    bool operator<(const State& b) const { return time>b.time; }
};
struct Edge { int to; double length; Edge(int t,double l):to(t),length(l){} };
int main() {
    ios::sync_with_stdio(false);
    string line; int cases;
    getline(cin,line); istringstream(line)>>cases;
    while(cases--) {
        map<string,int> cars, places;
        vector<double> speed; vector<int> available;
        while(getline(cin,line) && (line.empty() || line=="\r")) {}
        do {
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if(line.empty()) break;
            string name; double v; istringstream in(line); in>>name>>v;
            cars[name]=(int)speed.size(); speed.push_back(v);
        } while(getline(cin,line));
        while(getline(cin,line)) {
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if(line.empty()) break;
            string name,car; istringstream in(line); in>>name>>car;
            places[name]=(int)available.size(); available.push_back(cars[car]);
        }
        int n=(int)available.size(), m=(int)speed.size();
        vector< vector<Edge> > roads(n);
        while(getline(cin,line)) {
            istringstream in(line); string a,b; double length; in>>a;
            if(a=="*") break;
            if(!(in>>b>>length)) continue;
            int u=places[a], v=places[b];
            roads[u].push_back(Edge(v,length)); roads[v].push_back(Edge(u,length));
        }
        vector<double> dist(n*m,1e100);
        int start=places["PayPhone"], goal=places["WKCharriot"];
        priority_queue<State> q;
        dist[start*m+available[start]]=0;
        q.push(State(0,start,available[start]));
        double answer=-1;
        while(!q.empty()) {
            State s=q.top(); q.pop();
            if(s.time!=dist[s.place*m+s.car]) continue;
            if(s.place==goal) { answer=s.time; break; }
            int c=available[s.place];
            if(speed[c]>speed[s.car] && s.time+1<dist[s.place*m+c]) {
                dist[s.place*m+c]=s.time+1; q.push(State(s.time+1,s.place,c));
            }
            for(size_t i=0;i<roads[s.place].size();++i) {
                Edge e=roads[s.place][i]; double t=s.time+e.length*60.0/speed[s.car];
                if(t<dist[e.to*m+s.car]) {
                    dist[e.to*m+s.car]=t; q.push(State(t,e.to,s.car));
                }
            }
        }
        if(answer<0) puts("UNREACHABLE");
        else printf("%.3f\n",floor(answer*1000.0+1e-7)/1000.0);
    }
}
